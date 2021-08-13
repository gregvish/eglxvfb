#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

#include "xtest_event.h"
#include "eglxvfb.h"


void window_event_loop(Display *xdisplay, EGLXvfb_t *egl_xvfb)
{
    XEvent event = {0};
    xtest_event_t out_event = {0};

    while (true)
    {
        XNextEvent(xdisplay, &event);
        memset(&out_event, 0, sizeof(out_event));

        switch (event.type)
        {
            case MotionNotify:
                out_event.type = XTEST_EVENT_MOUSE;
                out_event.params[0] = EGLXvfb_normalize_x(
                    egl_xvfb, ((XMotionEvent*)&event)->x
                );
                out_event.params[1] = EGLXvfb_normalize_y(
                    egl_xvfb, ((XMotionEvent*)&event)->y
                );
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
                egl_xvfb->view_width = ((XConfigureEvent*)&event)->width;
                egl_xvfb->view_height = ((XConfigureEvent*)&event)->height;
                write(egl_xvfb->resize_fd, &((uint64_t[1]){1}), sizeof(uint64_t));
                break;

            default:
                break;
        }

        if (out_event.type != 0) {
            write(egl_xvfb->xtest_fd, &out_event, sizeof(out_event));
        }
    }
}


int main(int argc, const char *argv[])
{
    Window root = {0};
    Window win = {0};
    XSetWindowAttributes swa = {0};
    Display *xdisplay = NULL;
    pthread_t gl_thread = 0;
    EGLXvfb_t egl_xvfb = {0};

    if (argc != 2) {
        printf("Usage: ./xeglxvfb <Xvfb_dir_path>\n");
        return 1;
    }

    if (!EGLXvfb_connect(&egl_xvfb, argv[1])) {
        return 1;
    }

    xdisplay = XOpenDisplay(NULL);
    if (xdisplay == NULL) {
        printf("Error opening X display\n");
        return 1;
    }
    root = DefaultRootWindow(xdisplay);

    swa.event_mask = StructureNotifyMask | KeyPressMask | KeyReleaseMask |
                     ButtonPressMask | ButtonReleaseMask | PointerMotionMask;
    // TODO: XAllocSizeHints for window aspect ratio
    win = XCreateWindow(
        xdisplay, root, 0, 0, egl_xvfb.width, egl_xvfb.height, 0,
        CopyFromParent, InputOutput, CopyFromParent, CWEventMask, &swa
    );

    XMapWindow(xdisplay, win);
    XStoreName(xdisplay, win, "EGL");

    if (!EGLXvfb_init_egl(&egl_xvfb, xdisplay, win)) {
        printf("init_egl fail\n");
        return 1;
    }

    if (pthread_create(&gl_thread, NULL, EGLXvfb_gl_thread, &egl_xvfb)) {
        printf("pthread_create fail\n");
        return 1;
    }

    window_event_loop(xdisplay, &egl_xvfb);

    return 0;
}
