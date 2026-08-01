#ifndef _03_SINGLE_KNOB_H
#define _03_SINGLE_KNOB_H

#define SINGLE_KNOB_MODULE_ID 0b00000011

#define SINGLE_KNOB_INCREMENTS 117
#define SINGLE_KNOB_MAX_VALUE (SINGLE_KNOB_INCREMENTS - 1)

//Returns the value of the knob rotation (0 - 116)
uint8_t get_single_knob_rotation_value(module_t* module);
//Returns the ratio of the knob rotation (0.0 - 1.0)
float get_single_knob_rotation_ratio(module_t* module);

#include "03_single_knob.c"

#endif