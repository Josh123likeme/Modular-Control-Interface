#include <stdbool.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "uart.h"

bool error = false;
char error_reason[32];

//Stores the modules and their data. The order is arbitrary. This is effectively a pool of module data (This saves on malloc overhead).
module_t modules[MODULES_MAX_CACHED];
size_t modules_count = 0;

//Stores the modules that are connected (index 0 is the closest module to the controller).
module_t* connected_modules[MODULES_MAX_CONNECTED];
size_t connected_modules_count = 0;

//Stores the modules that have been seen since power on, but have been disconnected.
module_t* disconnected_modules[MODULES_MAX_CACHED];
size_t disconnected_modules_count = 0;

bool automatic_input_update_enabled = true;
bool automatic_output_update_enabled = true;

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

void refresh(void) {

    COMM_PORT |= MODE; //enable module discovery mode
    COMM_PORT &= ~ENABLE; //enable shift register input
    COMM_PORT |= ENABLE; //disable shift register input

    uint8_t module_ids_detected[MODULES_MAX_CONNECTED];
    size_t module_ids_detected_count = 0;

    while (true) {

        //reached the max number of modules
        if (module_ids_detected_count >= MODULES_MAX_CONNECTED) break;

        //receive module ID
        uint8_t module_id = receive_byte();
        
        //if module ID is 0 or 255 (all 0's or all 1's), there is no module connected
        if (module_id == 0 || module_id == 255) break;

        module_ids_detected[module_ids_detected_count] = module_id;
        module_ids_detected_count++;

    }

    COMM_PORT &= ~MODE;

    bool modules_unchanged = true;

    if (module_ids_detected_count != connected_modules_count) modules_unchanged = false;

    for (size_t i = 0; i < module_ids_detected_count; i++) {

        if ((*connected_modules[i]).module_id != module_ids_detected[i]) modules_unchanged = false;

    }

    //the modules have not changed, so don't bother with the re-arranging logic
    if (modules_unchanged) return;

    //this is probably the first refresh since powering on
    if (modules_count == 0 && connected_modules_count == 0) {

        for (size_t i = 0; i < module_ids_detected_count; i++) {

            module_t module;

            module.module_id = module_ids_detected[i];
            module.module_input_1 = 0x00;
            module.module_input_2 = 0x00;
            module.module_output = 0x00;

            modules[i] = module;

            connected_modules[i] = &modules[i];

        }

        modules_count = module_ids_detected_count;
        connected_modules_count = module_ids_detected_count;

        return;

    }

    //the modules have been re-arranged, so re-mapping is required

    //disconnect all modules
    for (int i = 0; i < connected_modules_count; i++) {

        if (disconnected_modules_count >= MODULES_MAX_CACHED) {

            error = true;
            strcpy(error_reason, "module cache full");
            return;
        }

        disconnected_modules[disconnected_modules_count++] = connected_modules[i];

    }

    connected_modules_count = 0;

    //reconnect modules
    for (int i = 0; i < module_ids_detected_count; i++) {

        bool module_found = false;

        //search for a module to go in its place
        for (int j = 0; j < disconnected_modules_count; j++) {

            //found a module to use
            if (disconnected_modules[j] != NULL && disconnected_modules[j]->module_id == module_ids_detected[i]) {

                module_found = true;

                connected_modules[connected_modules_count++] = disconnected_modules[j];
                disconnected_modules[j] = NULL;

                break;

            }
            
        }

        //didn't find a module to use, so create a new module in the cache and add it to the connected modules
        if (!module_found) {

            if (modules_count >= MODULES_MAX_CACHED) {

                error = true;
                strcpy(error_reason, "module cache full");
                return;
            }

            module_t* module_to_add = &modules[modules_count++];

            module_to_add->module_id = module_ids_detected[i];
            module_to_add->module_input_1 = 0x00;
            module_to_add->module_input_2 = 0x00;
            module_to_add->module_output = 0x00;

            connected_modules[connected_modules_count++] = module_to_add;

        }

    }

    //remove NULLs from disconnected modules tracker
    for (int i = 0; i < disconnected_modules_count;) {

        if (disconnected_modules[i] == NULL) {

            disconnected_modules[i] = disconnected_modules[--disconnected_modules_count];
            continue;

        }

        i++;

    }

    //update the module inputs to automatically set modules back to previous state
    update_module_inputs();

}

void enable_automatic_refreshing(void) {

    cli();

    TCCR1A = 0;
    TCCR1B = 0;

    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS11) | (1 << CS10);

    TIMSK1 |= (1 << OCIE1A);

    if (OCR1A == 0) OCR1A = 25000; //100ms by default if not already set

    sei();

}

void disable_automatic_refreshing(void) {

    cli();

    TCCR1B = 0;

    TIMSK1 &= ~(1 << OCIE1A);

    sei();

}

void set_refreshing_period(int ms) {

    cli();

    OCR1A = (250UL * ms) - 1;

    TCNT1 = 0;

    sei();

}

void update_module_inputs(void) {

    //transmit module inputs
    for (int i = connected_modules_count - 1; i >= 0; i--) {

        transmit_byte(connected_modules[i]->module_input_1);

    }

    //clock the data into the shift register output storage
    COMM_PORT |= SET;
    COMM_PORT &= ~SET;

    //transmit module inputs 2
    for (int i = connected_modules_count - 1; i >= 0; i--) {

        transmit_byte(connected_modules[i]->module_input_2);

    }

    //clock the data into the shift register output storage
    COMM_PORT |= SET;
    COMM_PORT &= ~SET;

}

void enable_automatic_input_update(void) {

    automatic_input_update_enabled = true;
}

void disable_automatic_input_update(void) {

    automatic_input_update_enabled = false;
}

void update_module_outputs(void) {

    //clock the data into the shift register
    COMM_PORT &= ~ENABLE;
    COMM_PORT |= ENABLE;

    //receive module outputs
    for (int i = 0; i < connected_modules_count; i++) {

        uint8_t byte = receive_byte();
        
        connected_modules[i]->module_output = byte;
        
    }

}

void enable_automatic_output_update(void) {

    automatic_output_update_enabled = true;
}

void disable_automatic_output_update(void) {

    automatic_output_update_enabled = false;
}

module_t* get_nth_occurrance(uint8_t module_id, size_t n) {

    size_t occurances_recorded = 0; //Number of occurances of this module

    for (size_t i = 0; i < connected_modules_count; i++) {

        if ((*connected_modules[i]).module_id == module_id) {
            
            if (occurances_recorded == n) return connected_modules[i];
            else occurances_recorded++;

        }

    }

    return NULL;

}

size_t get_number_of_connected_modules(void) {

    return connected_modules_count;

}

module_t* get_module_at(size_t index) {

    if (index >= connected_modules_count) return NULL;

    return connected_modules[index];

}

void print_module_summary(void) {

    put_str("\n\r\n\r----MODULE SUMMARY----\n\r");

    put_str("Connected modules: ");
    for (int i = 0; i < connected_modules_count; i++) {

        module_t* module = connected_modules[i];

        if (module == NULL) put_char('_');
        else put_num(module->module_id);

        put_char(',');
    }
    put_str("    (");
    put_num(connected_modules_count);
    put_str(")\n\r");

    put_str("Disconnected modules: ");
    for (int i = 0; i < disconnected_modules_count; i++) {

        module_t* module = disconnected_modules[i];

        if (module == NULL) put_char('_');
        else put_num(module->module_id);

        put_char(',');
    }
    put_str("    (");
    put_num(disconnected_modules_count);
    put_str(")\n\r");

    put_str("Module cache: ");
    for (int i = 0; i < modules_count; i++) {

        module_t module = modules[i];

        if (module.module_id == 0) put_char('_');
        else put_num(module.module_id);

        put_char(',');
    }
    put_str("    (");
    put_num(modules_count);
    put_str(")\n\r");

    put_str("ERRORS:\n\r");
    if (!error) put_str("NO ERRORS\n\r");
    else {
        put_str(error_reason);
        put_str("\n\r");
    }

    put_str("----------------------\n\r");

}

ISR(TIMER1_COMPA_vect) {
    
    refresh(); 
}