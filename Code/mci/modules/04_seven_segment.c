#include "../mci.h"

void set_seven_segment_hex(module_t* module, uint8_t hex) {

    if (module->module_id != SEVEN_SEGMENT_MODULE_ID) return;

    module->module_input_1 = pgm_read_byte(hex_segments + (hex & 0xF));
    module->module_input_2 = pgm_read_byte(hex_segments + (hex >> 4 & 0xF));

    if (automatic_input_update_enabled) update_module_inputs();

}

void set_seven_segment_integer(module_t* module, uint8_t integer) {

    if (module->module_id != SEVEN_SEGMENT_MODULE_ID) return;

    module->module_input_1 = pgm_read_byte(hex_segments + (integer % 10));
    module->module_input_2 = pgm_read_byte(hex_segments + (integer / 10));

    if (automatic_input_update_enabled) update_module_inputs();

}