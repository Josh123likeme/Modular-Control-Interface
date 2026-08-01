#include "mci.h"
#include "uart.h"
#include "modules/all.h"

#include <util/delay.h>

//this is a circular queue
#define UART_READ_QUEUE_MAX 128
uint8_t uart_read_queue[UART_READ_QUEUE_MAX];
volatile size_t uart_read_queue_front = 0; //index of the first value in the queue
volatile size_t uart_read_queue_back = 0; //index of the next available space, not the last value in the queue

static void handle_module_input_update_packet();
static void handle_module_output_request_packet();
static void handle_get_nth_occurrance_request_packet();
static void send_ready_signal();
static void echo_byte();

static uint8_t pop_byte_off_queue();
static void push_byte_on_queue(uint8_t byte);
static size_t get_uart_read_queue_count();

int main() {

    sei();

    init_uart();
    
    init_mci();

    refresh();

    set_refreshing_period(100);
    enable_automatic_refreshing();

    send_ready_signal();

    while (1) {
        
        uint8_t packet_id = pop_byte_off_queue();

        switch (packet_id) {

            case 0x00: handle_module_input_update_packet(); break;
            case 0x01: handle_module_output_request_packet(); break;
            case 0x02: handle_get_nth_occurrance_request_packet(); break;
            
            case 0x10: update_module_inputs(); break;
            case 0x11: enable_automatic_input_update(); break;
            case 0x12: disable_automatic_input_update(); break;

            case 0x20: update_module_outputs(); break;
            case 0x21: enable_automatic_output_update(); break;
            case 0x22: disable_automatic_output_update(); break;

            case 0xFF: echo_byte(); break;

        }

    }

}

static void handle_module_input_update_packet() {

    uint8_t module_pointer_high = pop_byte_off_queue();
    uint8_t module_pointer_low = pop_byte_off_queue();

    module_t* module = (module_t*) (module_pointer_high << 8 | module_pointer_low);

    uint8_t input_1 = pop_byte_off_queue();
    uint8_t input_2 = pop_byte_off_queue();

    module->module_input_1 = input_1;
    module->module_input_2 = input_2;

    if (automatic_input_update_enabled) update_module_inputs();

}

static void handle_module_output_request_packet() {

    uint8_t module_pointer_h = pop_byte_off_queue();
    uint8_t module_pointer_l = pop_byte_off_queue();

    module_t* module = (module_t*) (module_pointer_h << 8 | module_pointer_l);

    if (automatic_output_update_enabled) update_module_outputs();

    uint8_t output = module->module_output;

    put_byte(output);

}

static void handle_get_nth_occurrance_request_packet() {

    uint8_t module_id = pop_byte_off_queue();

    uint8_t n = pop_byte_off_queue();

    module_t* module = get_nth_occurrance(module_id, n);

    uint8_t module_pointer_h = (uint16_t) (module) >> 8 & 0xFF;
    uint8_t module_pointer_l = (uint16_t) (module) & 0xFF;

    put_byte(module_pointer_h);
    put_byte(module_pointer_l);

}

static void send_ready_signal() {

    put_byte('!');
    put_byte('r');
    put_byte('d');
    put_byte('y');

}

static void echo_byte() {

    uint8_t byte = pop_byte_off_queue();

    put_byte(byte);

}

static uint8_t pop_byte_off_queue() {

    //wait for byte to enter queue
    while (uart_read_queue_front == uart_read_queue_back);

    uint8_t byte = uart_read_queue[uart_read_queue_front];
    uart_read_queue_front++;
    if (uart_read_queue_front == UART_READ_QUEUE_MAX) uart_read_queue_front = 0;

    return byte;

}

static void push_byte_on_queue(uint8_t byte) {

    //detect full queue
    size_t next = uart_read_queue_back + 1;
    if (next == UART_READ_QUEUE_MAX) next = 0;
    if (next == uart_read_queue_front) return;

    uart_read_queue[uart_read_queue_back] = byte;
    uart_read_queue_back++;
    if (uart_read_queue_back == UART_READ_QUEUE_MAX) uart_read_queue_back = 0;

}

static size_t get_uart_read_queue_count() {

    if (uart_read_queue_back <= uart_read_queue_front) {

        return (UART_READ_QUEUE_MAX - uart_read_queue_front) + uart_read_queue_back;
    }
    else {

        return uart_read_queue_back - uart_read_queue_back;
    }

    return -1;

}

ISR(USART_RX_vect) {

    push_byte_on_queue(get_byte());

}