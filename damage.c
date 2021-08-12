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
#include <X11/extensions/Xfixes.h>
#include <X11/extensions/shape.h>

#include "damage_area.h"


typedef struct {
    damage_area_t damage_area;
    pthread_mutex_t lock;
    int damage_event_fd;
} damage_state_t;


static void *server_thread(void *arg)
{
    damage_state_t *state = (damage_state_t *)arg;
    uint64_t dummy = 0;

    while (true) {
        if (sizeof(uint8_t) != read(0, &dummy, sizeof(uint8_t))) {
            fprintf(stderr, "read fail\n");
            return NULL;
        }
        read(state->damage_event_fd, &dummy, sizeof(dummy));

        pthread_mutex_lock(&state->lock);

        if (sizeof(state->damage_area) !=
            write(1, &state->damage_area, sizeof(state->damage_area))) {
            fprintf(stderr, "write fail\n");
            return NULL;
        }

        memset(&state->damage_area, 0, sizeof(state->damage_area));
        pthread_mutex_unlock(&state->lock);
    }

    return NULL;
}


int main(void)
{
    int damage_event = 0;
    int damage_error = 0;
    int area_count = 0;
    int window_height __attribute__((unused)) = 0;
    int i = 0;
    Window root = {0};
    XWindowAttributes attributes = {0};
    XserverRegion region = {0};
    XEvent event = {0};
    XDamageNotifyEvent *dev = NULL;
    XRectangle *area = NULL;
    Display *display = NULL;
    damage_state_t state = {0};
    pthread_t serv_thread = 0;

    state.damage_event_fd = eventfd(0, 0);

    display = XOpenDisplay(NULL);
    if (!display) {
        fprintf(stderr, "XOpenDisplay fail.\n");
        return 1;
    }
    root = DefaultRootWindow(display);

    XGetWindowAttributes(display, root, &attributes);
    window_height = attributes.height;

    if (!XDamageQueryExtension(display, &damage_event, &damage_error)) {
        fprintf(stderr, "XDamageQueryExtension fail.\n");
        return 1;
    }
    XDamageCreate(display, root, XDamageReportNonEmpty);

    if (pthread_create(&serv_thread, NULL, server_thread, &state)) {
        fprintf(stderr, "pthread_create fail.\n");
        return 1;
    }

    while (true) {
        XNextEvent(display, &event);

        if (event.type == damage_event) {
            dev = (XDamageNotifyEvent *)&event;

            region = XFixesCreateRegion(display, NULL, 0);
            XDamageSubtract(display, dev->damage, None, region);
            area_count = 0;
            area = XFixesFetchRegion(display, region, &area_count);

            if (area) {
                pthread_mutex_lock(&state.lock);

                if (state.damage_area.end_y == 0) {
                    state.damage_area.y = area[0].y;
                    state.damage_area.end_y = area[0].y + area[0].height;
                }

                for (i = 0; i < area_count; i += 1) {
                    state.damage_area.y = MIN(state.damage_area.y, area[i].y);
                    state.damage_area.end_y = MAX(
                        state.damage_area.end_y,
                        area[i].y + area[i].height
                    );
                }

                write(state.damage_event_fd, &((uint64_t[1]){1}), sizeof(uint64_t));
                pthread_mutex_unlock(&state.lock);

                XFree(area);
            }

            XFixesDestroyRegion(display, region);
        }
    }

    XCloseDisplay(display);
    return 0;
}
