#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/socket.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>

#include "xtest_event.h"


int xtest_main(int fd)
{
    Display *display = NULL;
    xtest_event_t event = {0};
    KeyCode keycode = 0;

    display = XOpenDisplay(NULL);
    if (!display) {
        printf("XTest: XOpenDisplay fail.\n");
        return 1;
    }

    while (true) {
        if (sizeof(event) != recv(fd, &event, sizeof(event), 0)) {
            fprintf(stderr, "XTest: bad read\n");
            break;
        }

        switch (event.type) {
            case XTEST_EVENT_MOUSE:
                XTestFakeMotionEvent(
                    display, 0, event.params[0], event.params[1], CurrentTime
                );
                break;

            case XTEST_EVENT_BUTTON_PRESS:
                XTestFakeButtonEvent(display, event.params[0], true, CurrentTime);
                break;

            case XTEST_EVENT_BUTTON_RELEASE:
                XTestFakeButtonEvent(display, event.params[0], false, CurrentTime);
                break;

            case XTEST_EVENT_KEY_PRESS:
                keycode = XKeysymToKeycode(display, event.params[0]);
                XTestFakeKeyEvent(display, keycode, true, CurrentTime);
                break;

            case XTEST_EVENT_KEY_RELEASE:
                keycode = XKeysymToKeycode(display, event.params[0]);
                XTestFakeKeyEvent(display, keycode, false, CurrentTime);
                break;

            default:
                break;
        }

        XFlush(display);
    }

    XCloseDisplay(display);
    close(fd);
    fprintf(stderr, "XTest loop done.\n");

    return 0;
}
