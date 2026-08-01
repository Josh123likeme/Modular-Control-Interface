#include "mci.h"
#include "uart.h"
#include "modules/all.h"

#include <stddef.h>
#include <util/delay.h>

static void max_power_test(void);

int main() {

    init_uart();
    init_mci();
    
    //max_power_test();

    set_refreshing_period(100);
    enable_automatic_refreshing();

    uint8_t iter = 0;
    
    while (true) {

        iter++;
        if (iter > 99) iter = 0;

        update_module_outputs();
        
        module_t* seven_seg = get_nth_occurrance(SEVEN_SEGMENT_MODULE_ID, 0);
        module_t* single_knob = get_nth_occurrance(SINGLE_KNOB_MODULE_ID, 0);
        module_t* quad_button = get_nth_occurrance(QUAD_BUTTON_MODULE_ID, 0);
        module_t* quad_light = get_nth_occurrance(QUAD_LIGHT_MODULE_ID, 0);
        
        if (single_knob != NULL && seven_seg != NULL) set_seven_segment_integer(seven_seg, get_single_knob_rotation_value(single_knob));
        else if (single_knob == NULL && seven_seg != NULL) set_seven_segment_integer(seven_seg, iter);
        
        if (quad_button != NULL) {
            
            for (int i = 0; i < 4; i++) {

                set_quad_button_light_state(quad_button, i, get_quad_button_press_state(quad_button, i));

            }
            
        }

        if (quad_light != NULL) {

            for (int i = 0; i < 4; i++) {

                set_quad_light_state(quad_light, i, iter % 8);

            }

        }

        update_module_inputs();

        //print_module_summary();
        _delay_ms(100);

    }

}

static void max_power_test() {

    refresh();

    module_t* seven_seg = get_nth_occurrance(SEVEN_SEGMENT_MODULE_ID, 0);
    module_t* quad_button = get_nth_occurrance(QUAD_BUTTON_MODULE_ID, 0);
    module_t* quad_light = get_nth_occurrance(QUAD_LIGHT_MODULE_ID, 0);

    set_seven_segment_integer(seven_seg, 88);

    for (int i = 0; i < 4; i++) {

        set_quad_button_light_state(quad_button, i, 1);
        set_quad_light_state(quad_light, i, COLOUR_WHITE);

    }

    update_module_inputs();

    while (1);

}