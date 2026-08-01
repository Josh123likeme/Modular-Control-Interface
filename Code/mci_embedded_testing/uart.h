#ifndef UART_H
#define UART_H

#include <inttypes.h>
#include <stdlib.h>
#include <avr/io.h>

void init_uart(void);

char get_char(void);
void put_char(char ch);

void put_byte(uint8_t byte);

void put_str(char *str);

void put_num(int num);

#include "uart.c"

#endif