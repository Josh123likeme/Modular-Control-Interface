#include "../mci.h"

bool get_quad_button_press_state(module_t* module, uint8_t button_index) {

    if (module->module_id != QUAD_BUTTON_MODULE_ID) return NULL;
    if (button_index > 3) return NULL;

    if (automatic_output_update_enabled) update_module_outputs();
    
    return (module->module_output >> button_index) & 1;

}
bool get_quad_button_light_state(module_t* module, uint8_t button_index) {

    if (module->module_id != QUAD_BUTTON_MODULE_ID) return NULL;
    if (button_index > 3) return NULL;

    //Light state is only set by the mci controller and so module outputs do not need to be updated.
    //if (automatic_output_update_enabled) update_module_outputs();

    return (module->module_input_1 >> (7 - button_index)) & 1;

}

void set_quad_button_light_state(module_t* module, uint8_t button_index, bool state) {

    if (module->module_id != QUAD_BUTTON_MODULE_ID) return;
    if (button_index > 3) return;

    if (state) {
        module->module_input_1 |= (1 << button_index);
        module->module_input_2 |= (1 << button_index);
    }
    else {
        module->module_input_1 &= ~(1 << button_index);
        module->module_input_2 &= ~(1 << button_index);
    }

    if (automatic_input_update_enabled) update_module_inputs();

}