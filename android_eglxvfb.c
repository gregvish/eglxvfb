#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <pthread.h>

#include <EGL/egl.h>

#include <android_native_app_glue.h>

#include "xtest_event.h"
#include "keycode_xlate.h"
#include "eglxvfb.h"
#include "logging.h"


struct saved_state {
    int dummy;
};

struct engine {
    struct android_app* app;
    struct saved_state state;
    pthread_t gl_thread;
    EGLXvfb_t egl_xvfb;
    uint16_t last_mouse_button;
};


static int start_egl_thread(struct engine* engine)
{
    LOGI("start_egl_thread running");

    if (pthread_create(&engine->gl_thread, NULL, EGLXvfb_gl_thread, &engine->egl_xvfb)) {
        LOGW("pthread_create fail");
        return 1;
    }

    return 0;
}

static int engine_init_display(struct engine* engine)
{
    if (!EGLXvfb_connect(&engine->egl_xvfb, "/data/data/com.androideglxvfb/files/")) {
        LOGW("EGLXvfb_connect fail");
        return 1;
    }

    if (!EGLXvfb_init_egl(&engine->egl_xvfb, EGL_DEFAULT_DISPLAY, engine->app->window)) {
        LOGW("init_egl fail");
        return 1;
    }

    return 0;
}

static void engine_term_display(struct engine* engine)
{
}

static int32_t xlate_ButtonState(int32_t state)
{
    switch (state) {
        case 0:
        case AMOTION_EVENT_BUTTON_PRIMARY:
            return XTEST_BUTTON_LEFT;
        case AMOTION_EVENT_BUTTON_SECONDARY:
            return XTEST_BUTTON_RIGHT;
        case AMOTION_EVENT_BUTTON_TERTIARY:
            return XTEST_BUTTON_MIDDLE;
        default:
            LOGW("Bad ButtonState %d", state);
    }
    return 0;
}

static void xlate_MotionEvent(EGLXvfb_t *egl_xvfb, AInputEvent *event,
                              xtest_event_t *out_event)
{
    out_event->type = XTEST_EVENT_MOUSE;
    out_event->params[0] = EGLXvfb_normalize_x(egl_xvfb, AMotionEvent_getX(event, 0));
    out_event->params[1] = EGLXvfb_normalize_y(egl_xvfb, AMotionEvent_getY(event, 0));
}

static int32_t engine_handle_input(struct android_app* app, AInputEvent* event)
{
    struct engine* engine = (struct engine*)app->userData;
    int32_t key_val = 0;
    int32_t eventAction = 0;
    xtest_event_t out_event = {0};
    xtest_event_t out_event_sec = {0};

    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
        eventAction = AMotionEvent_getAction(event);
        switch (eventAction) {
            case AMOTION_EVENT_ACTION_UP:
                xlate_MotionEvent(&engine->egl_xvfb, event, &out_event);
                out_event_sec.type = XTEST_EVENT_BUTTON_RELEASE;
                out_event_sec.params[0] = engine->last_mouse_button;
                break;

            case AMOTION_EVENT_ACTION_DOWN:
                xlate_MotionEvent(&engine->egl_xvfb, event, &out_event);
                out_event_sec.type = XTEST_EVENT_BUTTON_PRESS;
                out_event_sec.params[0] =
                    xlate_ButtonState(AMotionEvent_getButtonState(event));
                engine->last_mouse_button = out_event_sec.params[0];
                break;

            case AMOTION_EVENT_ACTION_SCROLL:
                out_event.type = XTEST_EVENT_BUTTON_PRESS;
                out_event_sec.type = XTEST_EVENT_BUTTON_RELEASE;
                if (AMotionEvent_getAxisValue(event, AMOTION_EVENT_AXIS_VSCROLL, 0) > 0) {
                    out_event_sec.params[0] = out_event.params[0] = XTEST_BUTTON_SCROLL_UP;
                } else {
                    out_event_sec.params[0] = out_event.params[0] = XTEST_BUTTON_SCROLL_DOWN;
                }
                break;

            default:
                xlate_MotionEvent(&engine->egl_xvfb, event, &out_event);
                break;
        }

    } else if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_KEY) {
        key_val = AKeyEvent_getKeyCode(event);
        if (AKeyEvent_getAction(event) == AKEY_EVENT_ACTION_UP) {
            out_event.type = XTEST_EVENT_KEY_RELEASE;
            out_event.params[0] = android_to_x_keysym_xlate[key_val];

        } else if(AKeyEvent_getAction(event) == AKEY_EVENT_ACTION_DOWN) {
            out_event.type = XTEST_EVENT_KEY_PRESS;
            out_event.params[0] = android_to_x_keysym_xlate[key_val];
        }

    } else {
        return 0;
    }

    if (out_event.type != 0) {
        write(engine->egl_xvfb.xtest_fd, &out_event, sizeof(out_event));
    }
    if (out_event_sec.type != 0) {
        write(engine->egl_xvfb.xtest_fd, &out_event_sec, sizeof(out_event_sec));
    }

    return 1;
}

static void engine_handle_cmd(struct android_app* app, int32_t cmd)
{
    struct engine* engine = (struct engine*)app->userData;

    switch (cmd) {
        case APP_CMD_SAVE_STATE:
            engine->app->savedState = malloc(sizeof(struct saved_state));
            *((struct saved_state*)engine->app->savedState) = engine->state;
            engine->app->savedStateSize = sizeof(struct saved_state);
            break;

        case APP_CMD_INIT_WINDOW:
            if (engine->app->window != NULL) {
                engine_init_display(engine);
            }
            break;

        case APP_CMD_TERM_WINDOW:
            engine_term_display(engine);
            break;

        case APP_CMD_GAINED_FOCUS:
            start_egl_thread(engine);
            break;

        case APP_CMD_LOST_FOCUS:
            EGLXvfb_stop(&engine->egl_xvfb);
            break;
    }
}

void android_main(struct android_app* state)
{
    struct engine engine;

    memset(&engine, 0, sizeof(engine));
    state->userData = &engine;
    state->onAppCmd = engine_handle_cmd;
    state->onInputEvent = engine_handle_input;
    engine.app = state;

    if (state->savedState != NULL) {
        // We are starting with a previous saved state; restore from it.
        engine.state = *(struct saved_state*)state->savedState;
    }

    while (true) {
        int ident;
        int events;
        struct android_poll_source* source;

        while ((ident=ALooper_pollAll(-1, NULL, &events,
                                      (void**)&source)) >= 0) {

            if (source != NULL) {
                source->process(state, source);
            }

            // Check if we are exiting.
            if (state->destroyRequested != 0) {
                engine_term_display(&engine);
                return;
            }
        }
    }
}
