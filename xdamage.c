#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/socket.h>

#include <X11/Xlib.h>
#include <X11/extensions/Xdamage.h>


int xdamage_main(int fd)
{
    int damage_event = 0;
    int damage_error = 0;
    Window root = {0};
    XEvent event = {0};
    XDamageNotifyEvent *dev = NULL;
    Display *display = NULL;

    display = XOpenDisplay(NULL);
    if (!display) {
        fprintf(stderr, "Xdamage: XOpenDisplay fail.\n");
        return 1;
    }
    root = DefaultRootWindow(display);

    if (!XDamageQueryExtension(display, &damage_event, &damage_error)) {
        fprintf(stderr, "Xdamage: XDamageQueryExtension fail.\n");
        return 1;
    }
    XDamageCreate(display, root, XDamageReportNonEmpty);

    while (true) {
        XNextEvent(display, &event);

        if (event.type == damage_event) {
            dev = (XDamageNotifyEvent *)&event;

            XDamageSubtract(display, dev->damage, None, None);

            if (sizeof(uint8_t) != send(fd, "x", sizeof(uint8_t), MSG_NOSIGNAL)) {
                fprintf(stderr, "Xdamage: bad write\n");
                break;
            }
        }
    }

    XCloseDisplay(display);
    close(fd);
    fprintf(stderr, "XDamage loop done.\n");

    return 0;
}
