#ifndef __DAMAGE_AREA_H__
#define __DAMAGE_AREA_H__

#include <stdint.h>

typedef struct __attribute__((packed)) {
    uint16_t y;
    uint16_t end_y;
} damage_area_t;


#endif /* __DAMAGE_AREA_H__ */
