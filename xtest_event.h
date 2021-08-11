#ifndef __XTEST_EVENT_H__
#define __XTEST_EVENT_H__

#include <stdint.h>


typedef enum {
    XTEST_EVENT_MOUSE = 1,
    XTEST_EVENT_BUTTON_PRESS,
    XTEST_EVENT_BUTTON_RELEASE,
    XTEST_EVENT_KEY_PRESS,
    XTEST_EVENT_KEY_RELEASE,
} xtest_event_type_t;

typedef struct __attribute__((packed)) {
    xtest_event_type_t type;
    uint16_t params[2];
} xtest_event_t;


#endif /* __XTEST_EVENT_H__ */
