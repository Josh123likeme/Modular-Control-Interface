#ifndef _05_QUAD_LIGHT_H
#define _05_QUAD_LIGHT_H

#define QUAD_LIGHT_MODULE_ID 0b00000101

#define QUAD_LIGHT_TOP_LEFT 0
#define QUAD_LIGHT_TOP_RIGHT 1
#define QUAD_LIGHT_BOTTOM_LEFT 2
#define QUAD_LIGHT_BOTTOM_RIGHT 3

#define COLOUR_OFF 0b000
#define COLOUR_RED  0b001
#define COLOUR_GREEN 0b100
#define COLOUR_BLUE 0b010
#define COLOUR_YELLOW 0b101
#define COLOUR_CYAN 0b110
#define COLOUR_PURPLE 0b011
#define COLOUR_WHITE 0b111

void set_quad_light_state(module_t* module, uint8_t light_index, uint8_t colour);

#include "05_quad_light.c"

#endif