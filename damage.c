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

#include "damage_area.h"


typedef struct {
    damage_area_t damage_area;
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

        if (sizeof(state->damage_area) !=
            write(1, &state->damage_area, sizeof(state->damage_area))) {
            fprintf(stderr, "write fail\n");
            return NULL;
        }
    }

    return NULL;
}


int main(void)
{
    int damage_event = 0;
    int damage_error = 0;
    Window root = {0};
    XEvent event = {0};
    XDamageNotifyEvent *dev = NULL;
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

            XDamageSubtract(display, dev->damage, None, None);
            write(state.damage_event_fd, &((uint64_t[1]){1}), sizeof(uint64_t));
        }
    }

    XCloseDisplay(display);

    return 0;
}
