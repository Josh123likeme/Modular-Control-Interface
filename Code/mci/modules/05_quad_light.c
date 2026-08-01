#include "../mci.h"

void set_quad_light_state(module_t* module, uint8_t light_index, uint8_t colour) {

    if (module->module_id != QUAD_LIGHT_MODULE_ID) return;

    if (light_index > 3) return;

    switch (light_index) {

        case 0:
        module->module_input_2 &= 0b00000111;
        module->module_input_2 |= (colour & 0b111) << 4;
        break;

        case 1:
        module->module_input_2 &= 0b01110000;
        module->module_input_2 |= (colour & 0b111);
        break;

        case 2:
        module->module_input_1 &= 0b00000111;
        module->module_input_1 |= (colour & 0b111) << 4;
        break;

        case 3:
        module->module_input_1 &= 0b01110000;
        module->module_input_1 |= (colour & 0b111);
        break;

    }

    if (automatic_input_update_enabled) update_module_inputs();

}