#ifndef LOGIC_H
#define LOGIC_H
#include "memory.h"
#include "leds.h"

typedef enum{
    STAY_MODE,
    HUE_PICKER_MODE,
    SATURATION_PICKER_MODE,
    BRIGHTNESS_PICKER_MODE
} HSV_PICKER_MODES;

void read_state(uint32_t*);
void write_state(uint32_t*); 
#endif