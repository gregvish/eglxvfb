#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xdamage.h>
#include <X11/extensions/Xfixes.h>
#include <X11/extensions/shape.h>

#include "damage_area.h"


int main(void)
{
    int width = 0;
    int height = 0;
    int damage_event = 0;
    int damage_error = 0;
    int result = 0;
    int area_count = 0;
    int i = 0;
    uint8_t dummy = 0;

    Window root = {0};
    XWindowAttributes attributes = {0};
    Damage damage = {0};
    XserverRegion region = {0};
    XEvent event = {0};
    XDamageNotifyEvent *dev = NULL;
    XRectangle *area = NULL;
    Display *display = NULL;
    damage_area_t damage_area = {0};

    display = XOpenDisplay(NULL);
    if (!display) {
        fprintf(stderr, "XOpenDisplay fail.\n");
        return 1;
    }
    root = DefaultRootWindow(display);

    XGetWindowAttributes(display, root, &attributes);
    width = attributes.width;
    height = attributes.height;

    result = XDamageQueryExtension(display, &damage_event, &damage_error);
    damage = XDamageCreate(display, root, XDamageReportNonEmpty);

    while (true) {
        XNextEvent(display, &event);

        if (event.type == damage_event) {
            dev = (XDamageNotifyEvent *)&event;
            region = XFixesCreateRegion(display, NULL, 0);
            XDamageSubtract(display, dev->damage, None, region);
            area_count = 0;
            area = XFixesFetchRegion(display, region, &area_count);

            if (area) {
                for (i = 0; i < area_count; i += 1) {
                    damage_area.x = area[i].x;
                    damage_area.y = area[i].y;
                    damage_area.width = area[i].width;
                    damage_area.height = area[i].height;
                    write(1, &damage_area, sizeof(damage_area));
                }
                XFree(area);
            }
            XFixesDestroyRegion(display, region);
        }

        read(0, &dummy, 1);
    }

    XCloseDisplay(display);
    return 0;
}
