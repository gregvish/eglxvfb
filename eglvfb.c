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
#include <pthread.h>
#include <poll.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/XWDFile.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

#include "damage_area.h"
#include "xtest_event.h"


// EGL-related objects
EGLDisplay egl_display;
EGLConfig egl_conf;
EGLContext egl_context;
EGLSurface egl_surface;

PFNGLGENVERTEXARRAYSOESPROC glGenVertexArraysOES;
PFNGLBINDVERTEXARRAYOESPROC glBindVertexArrayOES;
PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArraysOES;
PFNGLISVERTEXARRAYOESPROC glIsVertexArrayOES;

int xtest_fd = -1;
int resize_fd = -1;
uint16_t view_width = 0;
uint16_t view_height = 0;
uint16_t display_width = 0;
uint16_t display_height = 0;


bool init_egl(EGLNativeDisplayType display, EGLNativeWindowType win)
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

    egl_display = eglGetDisplay(display);
    if (egl_display == EGL_NO_DISPLAY) {
        printf("Error getting EGL display\n");
        return false;
    }

    if (!eglInitialize(egl_display, &major, &minor)) {
        printf("Error initializing EGL\n");
        return false;
    }

    printf("EGL major: %d, minor %d\n", major, minor);

    if (!eglChooseConfig(egl_display, attr, &egl_conf, 1, &num_config)) {
        printf("Failed to choose config (eglError: %x)\n", eglGetError());
        return false;
    }

    if (num_config != 1) {
        return false;
    }

    egl_surface = eglCreateWindowSurface(egl_display, egl_conf, win, NULL);
    if (egl_surface == EGL_NO_SURFACE) {
        printf("CreateWindowSurface, EGL eglError: %d\n", eglGetError());
        return false;
    }

    egl_context = eglCreateContext(egl_display, egl_conf, EGL_NO_CONTEXT, ctxattr);
    if (egl_context == EGL_NO_CONTEXT) {
        printf("CreateContext, EGL eglError: %d\n", eglGetError());
        return false;
    }

    eglMakeCurrent(egl_display, egl_surface, egl_surface, egl_context);

    return true;
}


bool egl_get_extension_funcs(void)
{
    glGenVertexArraysOES = (PFNGLGENVERTEXARRAYSOESPROC)
        eglGetProcAddress("glGenVertexArraysOES");
    glBindVertexArrayOES = (PFNGLBINDVERTEXARRAYOESPROC)
        eglGetProcAddress("glBindVertexArrayOES");
    glDeleteVertexArraysOES = (PFNGLDELETEVERTEXARRAYSOESPROC)
        eglGetProcAddress("glDeleteVertexArraysOES");
    glIsVertexArrayOES = (PFNGLISVERTEXARRAYOESPROC)
        eglGetProcAddress("glIsVertexArrayOES");

    return true;
}


void gl_setup_scene()
{
    uint8_t err_info[1023] = {0};
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
    printf("glCompileShader (vertex): %s\n", err_info);

    // fragment shader
    int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

    glGetShaderInfoLog(fragment_shader, sizeof(err_info), &err_len, err_info);
    printf("glCompileShader (fragment): %s\n", err_info);

    // link shaders
    int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    glGetProgramInfoLog(shader_program, sizeof(err_info), &err_len, err_info);
    printf("glLinkProgram: %s\n", err_info);

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

    printf("GL error status (after glUseProgram): %d\n", glGetError());
}


void gl_draw_scene(GLuint texture)
{
    // clear
    glClearColor(0.0f, 0.1f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // draw quad
    // VAO and shader program are already bound from the call to gl_setup_scene
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}


int connect_to_uds(const char *path)
{
    struct sockaddr_un addr = {0};

    int sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sfd < 0) {
        printf("socket fail\n");
        return -1;
    }

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, path, sizeof(addr.sun_path) - 1);

    if (connect(sfd, (struct sockaddr *) &addr,
                sizeof(struct sockaddr_un)) == -1) {
        printf("connect fail to %s\n", path);
        return -1;
    }

    return sfd;
}


uint8_t *open_xvfb_shm(const char *path, uint16_t *width, uint16_t *height)
{
    int fd = 0;
    XWDFileHeader *header = NULL;
    uint16_t pixels_offset = 0;
    uint32_t size = 0;

    fd = open(path, O_RDONLY);
    if (fd < 0) {
        printf("Xvfb shm open fail\n");
        return NULL;
    }
    header = mmap(NULL, 4096, PROT_READ, MAP_SHARED, fd, 0);
    if (!header) {
        printf("mmap fail\n");
        return NULL;
    }

    pixels_offset = ntohl(header->header_size) + (ntohl(header->ncolors)) * sizeof(XWDColor);
    *width = ntohl(header->window_width);
    *height = ntohl(header->window_height);
    size = (*width) * (*height) * sizeof(uint32_t) + pixels_offset;

    munmap(header, 4096);
    header = mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0);
    if (!header) {
        printf("re-mmap fail\n");
        return NULL;
    }

    printf("Xvfb width: %d height: %d pixel data offset: %d\n",
           *width, *height, pixels_offset);

    return ((uint8_t *)header + pixels_offset);
}

void draw_loop(uint8_t *pixel_data, uint16_t width, uint16_t height, int damage_fd)
{
    GLuint texture = 0;
    damage_area_t damage_area = {0};
    uint64_t dummy = 0;
    struct pollfd pfds[] = {
        {.fd=damage_fd, .events=POLLIN},
        {.fd=resize_fd, .events=POLLIN}
    };

    gl_setup_scene();

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height,
                    GL_RGBA, GL_UNSIGNED_BYTE, pixel_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    while (true) {
        gl_draw_scene(texture);
        /* get rendered buffer to the screen */
        eglSwapBuffers(egl_display, egl_surface);

        write(damage_fd, &((uint8_t[]){0}), sizeof(uint8_t));

        /* check for xdamage or local window resize events */
        if (poll(pfds, sizeof(pfds) / sizeof(pfds[0]), -1) < 1) {
            printf("poll error\n");
            break;
        }
        if (pfds[1].revents & POLLIN) {
            read(resize_fd, &dummy, sizeof(dummy));
            glViewport(0, 0, view_width, view_height);
            continue;

        } else if (!(pfds[0].revents & POLLIN)) {
            printf("bad poll result\n");
            break;
        }

        read(damage_fd, &damage_area, sizeof(damage_area));
        glTexSubImage2D(
            GL_TEXTURE_2D, 0,
            0, damage_area.y, width, damage_area.height,
            GL_RGBA, GL_UNSIGNED_BYTE,
            pixel_data + (width * sizeof(uint32_t) * damage_area.y)
        );
    }
}


uint16_t normalize_width(uint16_t x)
{
    return (uint16_t)((float)display_width * ((float)x / (float)view_width));
}

uint16_t normalize_height(uint16_t y)
{
    return (uint16_t)((float)display_height * ((float)y / (float)view_height));
}


void *window_event_loop(void *arg)
{
    Display *xdisplay = (Display *)arg;
    XEvent event = {0};
    xtest_event_t out_event = {0};

    printf("Window event receiver thread running...\n");

    while (true)
    {
        XNextEvent(xdisplay, &event);
        memset(&out_event, 0, sizeof(out_event));

        switch (event.type)
        {
            case MotionNotify:
                out_event.type = XTEST_EVENT_MOUSE;
                out_event.params[0] = normalize_width(((XMotionEvent*)&event)->x);
                out_event.params[1] = normalize_height(((XMotionEvent*)&event)->y);
                break;

            case ButtonPress:
                out_event.type = XTEST_EVENT_BUTTON_PRESS;
                out_event.params[0] = ((XButtonEvent*)&event)->button;
                break;

            case ButtonRelease:
                out_event.type = XTEST_EVENT_BUTTON_RELEASE;
                out_event.params[0] = ((XButtonEvent*)&event)->button;
                break;

            case KeyPress:
                out_event.type = XTEST_EVENT_KEY_PRESS;
                out_event.params[0] = ((XKeyEvent*)&event)->keycode;
                break;

            case KeyRelease:
                out_event.type = XTEST_EVENT_KEY_RELEASE;
                out_event.params[0] = ((XKeyEvent*)&event)->keycode;
                break;

            case ConfigureNotify:
                view_width = ((XConfigureEvent*)&event)->width;
                view_height = ((XConfigureEvent*)&event)->height;
                write(resize_fd, &((uint64_t[1]){1}), sizeof(uint64_t));
                break;

            default:
                break;
        }

        if (out_event.type != 0) {
            write(xtest_fd, &out_event, sizeof(out_event));
        }
    }
}


int main()
{
    Window root = {0};
    Window win = {0};
    XSetWindowAttributes swa = {0};
    Display *xdisplay = NULL;
    uint8_t *pixel_data = NULL;
    uint16_t width = 0;
    uint16_t height = 0;
    int damage_fd = -1;
    pthread_t event_thread = 0;

    pixel_data = open_xvfb_shm("/tmp/kaka/Xvfb_screen0", &width, &height);
    damage_fd = connect_to_uds("/tmp/kaka/Xdamage");
    xtest_fd = connect_to_uds("/tmp/kaka/Xtest");
    resize_fd = eventfd(0, 0);

    if (!pixel_data || damage_fd < 0 || xtest_fd < 0 || resize_fd < 0) {
        return 1;
    }

    printf("pixel_data ptr: %p, damage fd: %d\n", pixel_data, damage_fd);

    xdisplay = XOpenDisplay(NULL);
    if (xdisplay == NULL) {
        printf("Error opening X display\n");
        return 0;
    }
    root = DefaultRootWindow(xdisplay);

    swa.event_mask = StructureNotifyMask | KeyPressMask | KeyReleaseMask |
                     ButtonPressMask | ButtonReleaseMask | PointerMotionMask;
    win = XCreateWindow(
        xdisplay, root, 0, 0, width, height, 0,
        CopyFromParent, InputOutput, CopyFromParent, CWEventMask, &swa
    );
    // TODO: XAllocSizeHints for window aspect ratio

    XMapWindow(xdisplay, win);
    XStoreName(xdisplay, win, "EGL");

    view_width = display_width = width;
    view_height = display_height = height;
    pthread_create(&event_thread, NULL, &window_event_loop, (void *)xdisplay);

    if (!init_egl(xdisplay, win) || !egl_get_extension_funcs()) {
        return 1;
    }

    draw_loop(pixel_data, width, height, damage_fd);

    return 0;
}
