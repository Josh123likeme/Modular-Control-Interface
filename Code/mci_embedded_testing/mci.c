static void transmit_byte(uint8_t byte);
static uint8_t receive_byte();

module_t modules[MODULES_MAX]; //allows for 16 modules
uint8_t modules_count;

void init_mci(void) {

    COMM_PORT = 0; //reset port

    //inputs
    COMM_DDR &= ~RX;

    //outputs
    COMM_DDR |= ENABLE;
    COMM_DDR |= MODE;
    COMM_DDR |= CLK;
    COMM_DDR |= SET;
    COMM_DDR |= TX;

}

void discover_modules(void) {

    put_str("Discovering modules...\n\r");

    COMM_PORT |= MODE; //enable module discovery mode
    COMM_PORT &= ~ENABLE; //enable shift register input
    COMM_PORT |= ENABLE; //disable shift register input

    //clear modules array
    modules_count = 0;

    while (true) {

        //reached the max number of modules
        if (modules_count >= MODULES_MAX) break;

        //receive module ID
        uint8_t module_id = receive_byte();
        
        //if module ID is 0 or 255 (all 0's or all 1's), there is no module connected
        if (module_id == 0 || module_id == 255) break;

        module_t module;

        module.module_id = module_id;
        module.module_input_1 = 0x00;
        module.module_input_2 = 0x00;
        module.module_output = 0x00;

        modules[modules_count] = module;

        modules_count++;

        put_str("Discovered module: ");
        put_byte(module_id);
        put_str("\n\r");

    }

    COMM_PORT &= ~MODE; //disable module discovery mode

}

void run_automatic_test(void) {

    uint8_t quad_button_index = index_of_first_module(MODULE_ID_QUAD_BUTTON);
    uint8_t single_knob_index = index_of_first_module(MODULE_ID_SINGLE_KNOB);
    uint8_t seven_segment_index = index_of_first_module(MODULE_ID_SEVEN_SEGMENT);
    uint8_t quad_light_index = index_of_first_module(MODULE_ID_QUAD_LIGHT);

    uint8_t seven_segment_display_number = 0;
    uint8_t quad_light_colours[] = {0, 0, 0, 0};

    if (quad_button_index != 255) put_str("Quad button is being tested\n\r");
    if (single_knob_index != 255) put_str("Single knob button is being tested\n\r");
    if (seven_segment_index != 255) put_str("Seven segment is being tested\n\r");
    if (quad_light_index != 255) put_str("Quad light is being tested\n\r");

    while (1) {

        update_module_outputs();

        //quad button testing. Holding a button down turns on the LED 
        if (quad_button_index != 255) {

            for (uint8_t i = 0; i < 4; i++) {

                bool state = get_quad_button_state(quad_button_index, i);
                
                set_quad_button_light(quad_button_index, i, state);

            }

            put_str("\n\r");

        }

        //quad light testing. All of the LEDs cycle through their colours
        if (quad_light_index != 255) {

            for (int i = 0; i < 4; i++) {

                quad_light_colours[i] = (quad_light_colours[i] + 1) % 8;

                set_quad_light_state(quad_light_index, i, quad_light_colours[0]);

            }

        }

        //single knob and seven segment testing. Seven segment displays output rotation of knob in hex
        if (single_knob_index != 255 && seven_segment_index != 255) {

            uint8_t rotation = get_single_knob_rotation(single_knob_index);

            set_seven_segment_hex(seven_segment_index, rotation);

        }

        //seven segment only test. Cycles number from 00-99
        if (seven_segment_index != 255 && single_knob_index == 255) {

            seven_segment_display_number = seven_segment_display_number == 99 ? 0 : seven_segment_display_number + 1;

            set_seven_segment_number(seven_segment_index, seven_segment_display_number);

        }

        update_module_inputs();

        _delay_ms(100);

    }

}

uint8_t index_of_first_module(int module_id) {

    for (int i = 0; i < modules_count; i++) {

        if (modules[i].module_id == module_id) return i;

    }

    return 255;

}

void update_module_inputs(void) {

    //transmit module inputs
    for (int i = modules_count - 1; i >= 0; i--) {

        transmit_byte(modules[i].module_input_1);

    }

    //clock the data into the shift register output storage
    COMM_PORT |= SET;
    COMM_PORT &= ~SET;

    //transmit module inputs 2
    for (int i = modules_count - 1; i >= 0; i--) {

        transmit_byte(modules[i].module_input_2);

    }

    //clock the data into the shift register output storage
    COMM_PORT |= SET;
    COMM_PORT &= ~SET;

}

void update_module_outputs(void) {

    //clock the data into the shift register
    COMM_PORT &= ~ENABLE;
    COMM_PORT |= ENABLE;

    //receive module outputs
    for (int i = 0; i < modules_count; i++) {

        uint8_t byte = receive_byte();

        modules[i].module_output = byte;

    }

}

void set_quad_button_light(uint8_t module_index, uint8_t light_index, bool state) {

    //module index outside array bounds
    if (module_index >= modules_count) return;

    module_t *module = &modules[module_index];

    //module is incorrect type
    if (module->module_id != MODULE_ID_QUAD_BUTTON) return;

    //light index out of bounds
    if (light_index > 3) return;

    //make the change to the module data structure
    if (state) {
        module->module_input_1 |= (1 << light_index);
        module->module_input_2 |= (1 << light_index);
    }
    else {
        module->module_input_1 &= ~(1 << light_index);
        module->module_input_2 &= ~(1 << light_index);
    }

}

bool get_quad_button_state(uint8_t module_index, uint8_t button_index) {

    //module index outside array bounds
    if (module_index >= modules_count) return false;

    module_t *module = &modules[module_index];

    //module is incorrect type
    if (module->module_id != MODULE_ID_QUAD_BUTTON) return false;

    //light index out of bounds
    if (button_index > 3) return false;

    return module->module_output >> button_index & 1;

}

void set_quad_light_state(uint8_t module_index, uint8_t light_index, uint8_t colour) {

    //module index outside array bounds
    if (module_index >= modules_count) return;

    module_t *module = &modules[module_index];

    //module is incorrect type
    if (module->module_id != MODULE_ID_QUAD_LIGHT) return;

    //light index out of bounds
    if (light_index > 3) return;

    //make the change to the module data structure
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

}

uint8_t get_single_knob_rotation(uint8_t module_index) {

    //module index outside array bounds
    if (module_index >= modules_count) return 255; //output is normally 0-127

    module_t *module = &modules[module_index];

    //module is incorrect type
    if (module->module_id != MODULE_ID_SINGLE_KNOB) return 255;

    uint8_t grey = module->module_output;

    uint8_t rotation = pgm_read_byte(encoder_map + grey);

    return rotation;
}

void set_seven_segment_hex(uint8_t module_index, uint8_t hex) {

    //module index outside array bounds
    if (module_index >= modules_count) return;

    module_t *module = &modules[module_index];

    //module is incorrect type
    if (module->module_id != MODULE_ID_SEVEN_SEGMENT) return;

    module->module_input_1 = pgm_read_byte(hex_segments + (hex & 0xF));

    module->module_input_2 = pgm_read_byte(hex_segments + (hex >> 4 & 0xF));

}

void set_seven_segment_number(uint8_t module_index, uint8_t number) {

    //module index outside array bounds
    if (module_index >= modules_count) return;

    module_t *module = &modules[module_index];

    //module is incorrect type
    if (module->module_id != MODULE_ID_SEVEN_SEGMENT) return;

    //number is too big to be displayed
    if (number > 99) return;

    module->module_input_1 = pgm_read_byte(hex_segments + (number % 10));

    module->module_input_2 = pgm_read_byte(hex_segments + (number / 10));

}

static void transmit_byte(uint8_t byte) {

    COMM_PORT &= CLK; //set clock low

    for (int i = 7; i >= 0; i--) {

        //set data bit
        if (byte >> i & 1) COMM_PORT |= TX;
        else COMM_PORT &= ~TX;

        //cycle clock
        COMM_PORT |= CLK;
        COMM_PORT &= ~CLK;

    }

}

static uint8_t receive_byte() {

    uint8_t byte = 0x00;
    
    COMM_PORT &= ~CLK; //set clock low
    
    for (int i = 7; i >= 0; i--) {

        byte |= (COMM_PIN & RX ? 1 : 0) << i; //receive bit

        //cycle clock
        COMM_PORT |= CLK;
        COMM_PORT &= ~CLK;

    }

    return byte;

}