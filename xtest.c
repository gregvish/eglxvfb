#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>

#include "xtest_event.h"


int main(void)
{
    Window root = {0};
    Display *display = NULL;
    xtest_event_t event = {0};

    display = XOpenDisplay(NULL);
    if (!display) {
        printf("XOpenDisplay fail.\n");
        return 1;
    }
    root = DefaultRootWindow(display);

    while (true) {
        if (sizeof(event) != read(0, &event, sizeof(event))) {
            fprintf(stderr, "bad read\n");
            return 1;
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
                XTestFakeKeyEvent(display, event.params[0], true, CurrentTime);
                break;

            case XTEST_EVENT_KEY_RELEASE:
                XTestFakeKeyEvent(display, event.params[0], false, CurrentTime);
                break;

            default:
                break;
        }

        XFlush(display);
    }

    XCloseDisplay(display);
    return 0;
}
