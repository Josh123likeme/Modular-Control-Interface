#ifndef MCI_H
#define MCI_H

#include <inttypes.h>
#include <stdbool.h>

#include <avr/io.h>

#include "rotary_encoder.h"
#include "seven_segment.h"
#include "uart.h"

#define MODULE_ID_NONE          0b00000000
#define MODULE_ID_QUAD_BUTTON   0b00000001
#define MODULE_ID_SINGLE_KNOB   0b00000011
#define MODULE_ID_SEVEN_SEGMENT 0b00000100
#define MODULE_ID_QUAD_LIGHT    0b00000101

#define MODULES_MAX 16

//DO NOT CHANGE THESE
#define COMM_PORT PORTC //communication port
#define COMM_PIN PINC //communication pin
#define COMM_DDR DDRC //communication port direction register
#define ENABLE 0b00000001 //enable pin (active low)
#define RX 0b00000010 //recieve data pin
#define MODE 0b00000100 //0 = normal operation, 1 = module discovery
#define CLK 0b00001000 //clock signal
#define SET 0b00010000 //set pin (active high)
#define TX 0b00100000 //transmit data pin

#define COLOUR_OFF 0b000
#define COLOUR_RED  0b100
#define COLOUR_GREEN 0b010
#define COLOUR_BLUE 0b001
#define COLOUR_YELLOW 0b110
#define COLOUR_CYAN 0b011
#define COLOUR_PURPLE 0b101
#define COLOUR_WHITE 0b111

//the module struct
typedef struct {

    uint8_t module_id;

    uint8_t module_input_1;
    uint8_t module_input_2;

    uint8_t module_output;

} module_t;

//intialies the mci controller
void init_mci(void);

//runs the module discovery process
void discover_modules(void);

//runs an automatic test based on the modules connected
void run_automatic_test(void);

//gets the index of the first occurrance of a particular module
uint8_t index_of_first_module(int module_id);

//updates all of the modules' inputs
void update_module_inputs(void);

//updates all of the modules' outputs
void update_module_outputs(void);

//quad button
void set_quad_button_light(uint8_t module_index, uint8_t light_index, bool state);
bool get_quad_button_state(uint8_t module_index, uint8_t button_index);

//quad light
void set_quad_light_state(uint8_t module_index, uint8_t light_index, uint8_t colour);

//single knob (0-127)
uint8_t get_single_knob_rotation(uint8_t module_index);

//seven segment
void set_seven_segment_hex(uint8_t module_index, uint8_t hex);
void set_seven_segment_number(uint8_t module_index, uint8_t number);

#include "mci.c"

#endif