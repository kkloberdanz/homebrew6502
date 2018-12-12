/*
 *  uart.c
 *
 *  A minimalist UART library
 *
 */

#include <avr/io.h>

#define BAUD_RATE 9600
#define BAUD_RATE_UBRR (F_CPU/16/BAUD_RATE-1)

void UART_init(void) {
	UBRR0H = (BAUD_RATE_UBRR >> 8); 
	UBRR0L = (BAUD_RATE_UBRR & 0xff); 

	UCSR0B = (1<<RXEN0)|(1<<TXEN0); // enable rx, tx 
	UCSR0C = (1<<USBS0)|(3<<UCSZ00); //(1<<UCSZ01)|(1<<UCSZ00)|(1<<URSEL0); 8n1
}

void UART_write(unsigned char ch) {
	while(!(UCSR0A&(1<<UDRE0)));
	UDR0=ch;
}

void UART_sendvalue(uint8_t value){
	char *hex = "0123456789ABCDEF";
	UART_write( hex[(value>>4)&0xf] );
	UART_write( hex[(value)&0xf] );
	UART_write(13);
	UART_write(10);
}

void UART_writestr( char *ch ) {
	while( *ch ) {
		UART_write( *ch );
		(void)*ch++;
	}
	UART_write(13);
	UART_write(10);
}
