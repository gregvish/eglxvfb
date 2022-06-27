#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/eventfd.h>
#include <arpa/inet.h>
#include <poll.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

#include "XWDFile.h"
#include "logging.h"
#include "eglxvfb.h"


PFNGLGENVERTEXARRAYSOESPROC glGenVertexArraysOES;
PFNGLBINDVERTEXARRAYOESPROC glBindVertexArrayOES;


static void egl_get_extension_funcs(void)
{
    if (!glGenVertexArraysOES) {
        glGenVertexArraysOES = (PFNGLGENVERTEXARRAYSOESPROC)
            eglGetProcAddress("glGenVertexArraysOES");
    }
    assert(glGenVertexArraysOES);

    if (!glBindVertexArrayOES) {
        glBindVertexArrayOES = (PFNGLBINDVERTEXARRAYOESPROC)
            eglGetProcAddress("glBindVertexArrayOES");
    }
    assert(glBindVertexArrayOES);
}


bool EGLXvfb_init_egl(EGLXvfb_t *self,
                      EGLNativeDisplayType display,
                      EGLNativeWindowType win)
{
    EGLint attr[] = {
        EGL_SURFACE_TYPE,           EGL_WINDOW_BIT,
        EGL_RED_SIZE,               8,
        EGL_GREEN_SIZE,             8,
        EGL_BLUE_SIZE,              8,
        EGL_RENDERABLE_TYPE,        EGL_OPENGL_ES2_BIT,
        EGL_NONE
    };

    EGLint num_config = 0;
    EGLint major = 0;
    EGLint minor = 0;

    EGLint ctxattr[] = {
       EGL_CONTEXT_CLIENT_VERSION, 2,
       EGL_NONE
    };

    self->egl_display = eglGetDisplay(display);
    if (self->egl_display == EGL_NO_DISPLAY) {
        LOGW("Error getting EGL display\n");
        return false;
    }

    if (!eglInitialize(self->egl_display, &major, &minor)) {
        LOGW("Error initializing EGL\n");
        return false;
    }

    LOGI("EGL major: %d, minor %d\n", major, minor);

    if (!eglChooseConfig(self->egl_display, attr, &self->egl_conf, 1, &num_config)) {
        LOGW("Failed to choose config (eglError: %x)\n", eglGetError());
        return false;
    }

    if (num_config != 1) {
        return false;
    }

    self->egl_surface = eglCreateWindowSurface(
        self->egl_display, self->egl_conf, win, NULL
    );
    if (self->egl_surface == EGL_NO_SURFACE) {
        LOGW("CreateWindowSurface, EGL eglError: %d\n", eglGetError());
        return false;
    }

    self->egl_context = eglCreateContext(
        self->egl_display, self->egl_conf, EGL_NO_CONTEXT, ctxattr
    );
    if (self->egl_context == EGL_NO_CONTEXT) {
        LOGW("CreateContext, EGL eglError: %d\n", eglGetError());
        return false;
    }

    eglMakeCurrent(
        self->egl_display, self->egl_surface, self->egl_surface, self->egl_context
    );
    egl_get_extension_funcs();
    eglMakeCurrent(self->egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

    return true;
}


static void gl_setup_scene()
{
    GLchar err_info[1024] = {0};
    int err_len = 0;

    // Shader source that draws a textures quad
    const char *vertex_shader_source = (
        "#version 300 es\n"
        "precision mediump float;\n"

        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec2 aTexCoords;\n"
        "out vec2 TexCoords;\n"

        "void main()\n"
        "{\n"
        "   TexCoords = aTexCoords;\n"
        "   gl_Position = vec4(-aPos.y, aPos.x, aPos.z, 1.0);\n"
        "}\0"
    );
    const char *fragment_shader_source = (
        "#version 300 es\n"
        "precision mediump float;\n"

        "out vec4 FragColor;\n"
        "in vec2 TexCoords;\n"
        "uniform sampler2D Texture1;\n"

        "void main()\n"
        "{\n"
        "   FragColor.xyzw = texture(Texture1, TexCoords).zyxw;\n"
        "}\0"
    );

    // vertex shader
    int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

    glGetShaderInfoLog(vertex_shader, sizeof(err_info), &err_len, err_info);
    LOGI("glCompileShader (vertex): %s\n", err_info);

    // fragment shader
    int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

    glGetShaderInfoLog(fragment_shader, sizeof(err_info), &err_len, err_info);
    LOGI("glCompileShader (fragment): %s\n", err_info);

    // link shaders
    int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    glGetProgramInfoLog(shader_program, sizeof(err_info), &err_len, err_info);
    LOGI("glLinkProgram: %s\n", err_info);

    // delete shaders
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // quad
    float vertices[] = {
        1.0f, 1.0f, 0.0f, 0.0f, 0.0f,   // top right
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, // bottom left
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
    };
    unsigned int indices[] = {
        0, 1, 3, // first Triangle
        1, 2, 3  // second Triangle
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArraysOES(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArrayOES(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                          5 * sizeof(float), (void *)(3 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArrayOES(0);

    // Prebind needed stuff for drawing
    glUseProgram(shader_program);
    glBindVertexArrayOES(VAO);

    LOGI("GL error status (after glUseProgram): %d\n", glGetError());
}


static void gl_draw_scene(GLuint texture)
{
    // clear
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // draw quad
    // VAO and shader program are already bound from the call to gl_setup_scene
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}


static int connect_to_abstract_uds(const char *path)
{
    struct sockaddr_un addr = {0};
    uint16_t addrlen = 0;

    int sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sfd < 0) {
        LOGW("socket fail\n");
        return -1;
    }

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path + 1, path, sizeof(addr.sun_path) - 2);
    // Abstract namespace (first path byte is NULL, and addrlen must be shortened)
    addr.sun_path[0] = 0;
    addrlen = sizeof(addr) - (sizeof(addr.sun_path) - strlen(addr.sun_path + 1) - 1);

    if (connect(sfd, (struct sockaddr *)&addr, addrlen) == -1) {
        LOGW("connect fail to %s\n", path);
        return -1;
    }

    return sfd;
}


bool read_fd_from_uds(int sock, int *fd, void *data, size_t data_len)
{
    struct msghdr msg = {0};
    struct iovec io = {.iov_base = data, .iov_len = data_len};
    char c_buffer[256] = {0};
    struct cmsghdr *cmsg = NULL;

    msg.msg_iov = &io;
    msg.msg_iovlen = 1;
    msg.msg_control = c_buffer;
    msg.msg_controllen = sizeof(c_buffer);

    if (recvmsg(sock, &msg, 0) < 0) {
        return false;
    }

    cmsg = CMSG_FIRSTHDR(&msg);
    if (!cmsg) {
        LOGW("No CMSG attached\n");
        return false;
    }

    memmove(fd, CMSG_DATA(cmsg), sizeof(*fd));
    return true;
}


static uint8_t *get_xvfb_shm_and_fds(EGLXvfb_t *self, const char *path,
                                     int *xtest_fd_ptr, int *xdamage_fd_ptr)
{
    int sock_fd = -1;
    XWDFileHeader *header = NULL;
    uint16_t pixels_offset = 0;
    uint8_t dummy = 0;
    uint8_t *retval = NULL;

    sock_fd = connect_to_abstract_uds(path);
    if (sock_fd < 0) {
        LOGW("Xvfb UDS connect fail\n");
        goto Cleanup;
    }

    if (!read_fd_from_uds(sock_fd, &self->shm_fd, &dummy, sizeof(dummy)) ||
        !read_fd_from_uds(sock_fd, xtest_fd_ptr, &dummy, sizeof(dummy)) ||
        !read_fd_from_uds(sock_fd, xdamage_fd_ptr, &dummy, sizeof(dummy))) {
        LOGW("Xvfb read fds from UDS fail\n");
        goto Cleanup;
    }

    header = mmap(NULL, 4096, PROT_READ, MAP_SHARED, self->shm_fd, 0);
    if (!header) {
        LOGW("mmap fail\n");
        goto Cleanup;
    }

    pixels_offset = ntohl(header->header_size) + (ntohl(header->ncolors)) * sizeof(XWDColor);
    self->view_width = self->width = ntohl(header->window_width);
    self->view_height = self->height = ntohl(header->window_height);
    self->map_size = self->width * self->height * sizeof(uint32_t) + pixels_offset;

    munmap(header, 4096);
    header = mmap(NULL, self->map_size, PROT_READ, MAP_SHARED, self->shm_fd, 0);
    if (!header) {
        LOGW("re-mmap fail\n");
        goto Cleanup;
    }

    LOGI("Xvfb width: %d height: %d pixel data offset: %d\n",
           self->width, self->height, pixels_offset);

    retval = ((uint8_t *)header + pixels_offset);

Cleanup:
    if (sock_fd > 0) {
        close(sock_fd);
    }

    return retval;
}


bool EGLXvfb_connect(EGLXvfb_t *self, const char *dir)
{
    char path[1024] = {0};

    snprintf(path, sizeof(path), "%s/Xvfb_shm", dir);
    self->pixel_data = get_xvfb_shm_and_fds(self, path, &self->xtest_fd, &self->xdamage_fd);
    self->resize_fd = eventfd(0, 0);

    if (!self->pixel_data || self->xdamage_fd < 0 ||
        self->xtest_fd < 0 || self->resize_fd < 0) {
        return false;
    }

    LOGI("pixel_data ptr: %p, xdamage fd: %d, xtest fd: %d\n",
           (void *)self->pixel_data, self->xdamage_fd, self->xtest_fd);
    return true;
}


static void draw_loop(EGLXvfb_t *self)
{
    GLuint texture = 0;
    uint64_t dummy = 0;
    uint8_t dummy_buf[128] = {0};
    struct pollfd pfds[] = {
        {.fd=self->xdamage_fd, .events=POLLIN},
        {.fd=self->resize_fd, .events=POLLIN}
    };

    gl_setup_scene();

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, self->width, self->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, self->width, self->height,
                    GL_RGBA, GL_UNSIGNED_BYTE, self->pixel_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    LOGI("GL error after texture setup: %d\n", glGetError());

    glViewport(0, 0, self->view_width, self->view_height);

    while (self->running) {
        gl_draw_scene(texture);
        /* get rendered buffer to the screen */
        eglSwapBuffers(self->egl_display, self->egl_surface);

        /* check for xdamage or local window resize events */
        if (poll(pfds, sizeof(pfds) / sizeof(pfds[0]), -1) < 1) {
            LOGW("poll error\n");
            break;
        }
        if (pfds[1].revents & POLLIN) {
            read(self->resize_fd, &dummy, sizeof(dummy));
            glViewport(0, 0, self->view_width, self->view_height);
            continue;

        } else if (!(pfds[0].revents & POLLIN)) {
            LOGW("bad poll result\n");
            break;
        }

        read(self->xdamage_fd, &dummy_buf, sizeof(dummy_buf));

        glTexSubImage2D(
            GL_TEXTURE_2D, 0,
            0, 0, self->width, self->height,
            GL_RGBA, GL_UNSIGNED_BYTE,
            self->pixel_data
        );
    }

    LOGI("GL draw loop ended: %d\n", glGetError());
}


uint16_t EGLXvfb_normalize_x(EGLXvfb_t *self, uint16_t x)
{
    return (uint16_t)(self->width * ((float)(x) / self->view_width));
}


uint16_t EGLXvfb_normalize_y(EGLXvfb_t *self, uint16_t y)
{
    return (uint16_t)(self->height * ((float)(y) / self->view_height));
}


void EGLXvfb_cleanup(EGLXvfb_t *self)
{
    if (self->xdamage_fd > 0) {
        close(self->xdamage_fd);
    }
    if (self->xtest_fd > 0) {
        close(self->xtest_fd);
    }
    if (self->resize_fd > 0) {
        close(self->resize_fd);
    }
    if (self->pixel_data) {
        munmap(self->pixel_data, self->map_size);
    }
    if (self->shm_fd > 0) {
        close(self->shm_fd);
    }
    if (self->egl_context) {
        eglDestroyContext(self->egl_display, self->egl_context);
    }
    if (self->egl_surface) {
        eglDestroySurface(self->egl_display, self->egl_surface);
    }
}

void *EGLXvfb_gl_thread(void *arg)
{
    EGLXvfb_t *self = (EGLXvfb_t *)arg;
    EGLContext child_egl_ctx = 0;
    EGLint ctxattr[] = {
       EGL_CONTEXT_CLIENT_VERSION, 2,
       EGL_NONE
    };

    child_egl_ctx = eglCreateContext(
        self->egl_display, self->egl_conf, self->egl_context, ctxattr
    );
    if (child_egl_ctx == EGL_NO_CONTEXT) {
        LOGW("CreateContext (child context), EGL eglError: %d\n", eglGetError());
        return NULL;
    }

    eglMakeCurrent(
        self->egl_display, self->egl_surface, self->egl_surface, child_egl_ctx
    );

    self->running = true;
    draw_loop(self);

    eglDestroyContext(self->egl_display, child_egl_ctx);
    return NULL;
}

void EGLXvfb_stop(EGLXvfb_t *self)
{
    self->running = false;
    write(self->resize_fd, &((uint64_t[1]){1}), sizeof(uint64_t));
}

