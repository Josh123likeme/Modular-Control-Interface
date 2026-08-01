#ifndef _04_SEVEN_SEGMENT_H
#define _04_SEVEN_SEGMENT_H

#define SEVEN_SEGMENT_MODULE_ID 0b00000100

#include <avr/pgmspace.h>

const uint8_t hex_segments[16] PROGMEM = {
  0b00111111,
  0b00000110,
  0b01011011,
  0b01001111,
  0b01100110,
  0b01101101,
  0b01111101,
  0b00000111,
  0b01111111,
  0b01100111,
  0b01110111,
  0b01111100,
  0b00111001,
  0b01011110,
  0b01111001,
  0b01110001};

void set_seven_segment_hex(module_t* module, uint8_t hex);
void set_seven_segment_integer(module_t* module, uint8_t integer);

#include "04_seven_segment.c"

#endif