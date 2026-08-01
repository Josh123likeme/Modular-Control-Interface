#ifndef _01_QUAD_BUTTON_H
#define _01_QUAD_BUTTON_H

#define QUAD_BUTTON_MODULE_ID 0b00000001

#define QUAD_BUTTON_TOP_LEFT 0
#define QUAD_BUTTON_TOP_RIGHT 1
#define QUAD_BUTTON_BOTTOM_LEFT 2
#define QUAD_BUTTON_BOTTOM_RIGHT 3
#define QUAD_BUTTON_LIGHT_OFF 0
#define QUAD_BUTTON_LIGHT_ON 1

bool get_quad_button_press_state(module_t* module, uint8_t button_index);
bool get_quad_button_light_state(module_t* module, uint8_t button_index);

void set_quad_button_light_state(module_t* module, uint8_t button_index, bool state);

#include "01_quad_button.c"

#endif