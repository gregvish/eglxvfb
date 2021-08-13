#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <pthread.h>

#include <EGL/egl.h>

#include <android_native_app_glue.h>

#include "xtest_event.h"
#include "eglxvfb.h"
#include "logging.h"


struct saved_state {
    int32_t x;
    int32_t y;
};

struct engine {
    struct android_app* app;
    struct saved_state state;
    int animating;
    pthread_t gl_thread;
    EGLXvfb_t egl_xvfb;
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
        LOGW("init_egl fail\n");
        return 1;
    }

    return 0;
}

static void engine_term_display(struct engine* engine)
{
    engine->animating = 0;
}

static int32_t engine_handle_input(struct android_app* app, AInputEvent* event)
{
    struct engine* engine = (struct engine*)app->userData;

    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
        engine->animating = 1;
        engine->state.x = AMotionEvent_getX(event, 0);
        engine->state.y = AMotionEvent_getY(event, 0);
        return 1;
    }

    return 0;
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
            engine->animating = 1;
            start_egl_thread(engine);
            break;

        case APP_CMD_LOST_FOCUS:
            EGLXvfb_stop(&engine->egl_xvfb);
            engine->animating = 0;
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

        // If not animating, we will block forever waiting for events.
        // If animating, we loop until all events are read, then continue
        // to draw the next frame of animation.
        while ((ident=ALooper_pollAll(engine.animating ? 0 : -1, NULL, &events,
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
