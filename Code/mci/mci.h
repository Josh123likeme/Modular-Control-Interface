#ifndef MCI_H
#define MCI_H

#include <inttypes.h>
#include <stddef.h>

#define MODULES_MAX_CACHED 32 //The max number of modules that can be cached
#define MODULES_MAX_CONNECTED 16 //The max number of modules that can be connected

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

//the module struct
typedef struct {

    uint8_t module_id;

    uint8_t module_input_1;
    uint8_t module_input_2;

    uint8_t module_output;

} module_t;

/*
Initialises the MCI system
*/
void init_mci(void);

/*
Refresh loaded modules.
If modules haven't changed, this function does nothing.
Use when re-arranging the modules.
*/
void refresh(void);
/*
Enables/Disables automatic refreshing.
Enabled: An update period needs to be specified using `set_refreshing_period()` or `set_refreshing_frequency()`.
Disabled: `refresh()` function must be manually called.
*/
void enable_automatic_refreshing(void);
void disable_automatic_refreshing(void);

void set_refreshing_period(int ms);

//Updates module inputs.
void update_module_inputs(void);
/*
Enables/Disables automatic module input updating.
Enabled: The module inputs will update automatically when ever an input is changed.
Disabled: The module inputs will only update when `update_module_inputs()` function is manually called.
*/
void enable_automatic_input_update(void);
void disable_automatic_input_update(void);

//Updates module outputs.
void update_module_outputs(void);
/*
Enables/Disables automatic module output updating.
Enabled: The module outputs will update automatically when ever an output is requested.
Disabled: The module outputs will only update when `update_module_outputs()` function is manually called.
*/
void enable_automatic_output_update(void);
void disable_automatic_output_update(void);

//Returns a pointer to the nth occurance of a particular module id. The first occurance is at n=0.
module_t* get_nth_occurrance(uint8_t module_id, size_t n);

//Returns the number of currently connected modules.
size_t get_number_of_connected_modules(void);

//Returns a pointer to the module at the real-life index.
module_t* get_module_at(size_t index);

//Prints the module summary.
void print_module_summary(void);

#include "mci.c"

#endif