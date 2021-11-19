#ifndef __LOGGING_H__
#define __LOGGING_H__

#ifdef ANDROID_BUILD
    #include <android/log.h>

    #define LOGI(...) ((void)__android_log_print( \
        ANDROID_LOG_INFO, "AndroidEGLXvfb", __VA_ARGS__))
    #define LOGW(...) ((void)__android_log_print( \
        ANDROID_LOG_WARN, "AndroidEGLXvfb", __VA_ARGS__))

#else /* Not ANDROID_BUILD */

    #define LOGI(...) fprintf(stderr, "[EGLXvfb] " __VA_ARGS__)
    #define LOGW(...) fprintf(stderr, "[EGLXvfb] " __VA_ARGS__)

#endif /* ANDROID_BUILD */

#endif /* __LOGGING_H__ */

