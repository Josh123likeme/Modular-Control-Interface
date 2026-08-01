void init_uart(void) {

	/* Configure 9600 baud, 8-bit, no parity and one stop bit */
	const int baud_rate = 9600;

	UBRR0H = (F_CPU/(baud_rate*16L)-1) >> 8;
	UBRR0L = (F_CPU/(baud_rate*16L)-1);
	UCSR0B = _BV(RXEN0) | _BV(TXEN0) | _BV(RXCIE0);
	UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);

	sei();
}

char get_char(void) {

	while(!(UCSR0A & _BV(RXC0)));

	return UDR0;
}

void put_char(char ch) {

	while (!(UCSR0A & _BV(UDRE0)));

	UDR0 = ch;
}

uint8_t get_byte(void) {

	while(!(UCSR0A & _BV(RXC0)));

	return UDR0;
}

void put_byte(uint8_t byte) {

	while (!(UCSR0A & _BV(UDRE0)));

	UDR0 = byte;

}

void put_str(char *str) {

	for(int i = 0; str[i]; i++) put_char(str[i]);
}

void put_num(int num) {

	char str[20];

	itoa(num, str, 10);

	put_str(str);

}