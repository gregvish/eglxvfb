#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/eventfd.h>
#include <sys/param.h>
#include <pthread.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xdamage.h>


int main(void)
{
    int damage_event = 0;
    int damage_error = 0;
    Window root = {0};
    XEvent event = {0};
    XDamageNotifyEvent *dev = NULL;
    Display *display = NULL;

    display = XOpenDisplay(NULL);
    if (!display) {
        fprintf(stderr, "XOpenDisplay fail.\n");
        return 1;
    }
    root = DefaultRootWindow(display);

    if (!XDamageQueryExtension(display, &damage_event, &damage_error)) {
        fprintf(stderr, "XDamageQueryExtension fail.\n");
        return 1;
    }
    XDamageCreate(display, root, XDamageReportNonEmpty);

    while (true) {
        XNextEvent(display, &event);

        if (event.type == damage_event) {
            dev = (XDamageNotifyEvent *)&event;

            XDamageSubtract(display, dev->damage, None, None);
            write(1, "x", sizeof(uint8_t));
        }
    }

    XCloseDisplay(display);

    return 0;
}
