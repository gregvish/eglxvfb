#ifndef __EGLXVFB_H__
#define __EGLXVFB_H__

#include <stdint.h>
#include <EGL/egl.h>


typedef struct {
    EGLDisplay egl_display;
    EGLConfig egl_conf;
    EGLContext egl_context;
    EGLSurface egl_surface;
    int xdamage_fd;
    int xtest_fd;
    int resize_fd;
    int shm_fd;
    uint16_t width;
    uint16_t height;
    uint16_t view_width;
    uint16_t view_height;
    uint8_t *pixel_data;
    bool running;
    uint32_t map_size;
} EGLXvfb_t;


bool EGLXvfb_connect(EGLXvfb_t *self, const char *dir);
bool EGLXvfb_init_egl(EGLXvfb_t *self,
                      EGLNativeDisplayType display,
                      EGLNativeWindowType win);
void *EGLXvfb_gl_thread(void *arg);
uint16_t EGLXvfb_normalize_x(EGLXvfb_t *self, uint16_t x);
uint16_t EGLXvfb_normalize_y(EGLXvfb_t *self, uint16_t y);
void EGLXvfb_stop(EGLXvfb_t *self);
void EGLXvfb_cleanup(EGLXvfb_t *self);


#endif /* __EGLXVFB_H__ */
