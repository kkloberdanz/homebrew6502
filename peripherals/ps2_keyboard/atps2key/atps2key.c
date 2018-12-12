/*
 * ATPS2Key - ATmega PS/2 keyboard pulse to scancode driver.
 * Developed on ATmega128, but easily portable to other controllers within the Atmel family.
 * Written by Stian Soreng, http://jmp.no/atvideotext/
 *
 * (C)2012 released under the GNU General Public License, see
 *         http://gnu.org/licenses/licenses.html for details.
 *
 * This program converts PS/2 data pulses to scancodes.
 * It also transmits a translated scancode via UART0 (9600/8N1) for debugging purposes.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "keyboardlookup.h"

ISR(SIG_INTERRUPT0) {
	static uint8_t bitcounter=0;
	uint8_t pb = PINB&1;
	static uint8_t byte=0;
	static uint8_t skipnextchar=0;

	if( bitcounter < 10 ) {
		byte |= (pb<<(bitcounter-1));
	} else {
		switch( byte ) {
			case 0x00:
				// do nothing
				break;
			case 0xf0:
				skipnextchar=1;		// Avoid printing the char again
				// key up
				break;
			case 0x5a:
				UART_write(10);
				UART_write(13);
				break;
			default:
				if( ! skipnextchar )
					UART_write( keyboardlookup[byte] );
				skipnextchar=0;
				break;
		}
		byte = 0;
		bitcounter = -1;
	}
	bitcounter++;
}

int main(void) {
	DDRB = 0;
	DDRD = 0;

	UART_init();
	UART_writestr( "Keyboard is ready, start typing" );

	EIMSK |= _BV(INT0);
	EICRA |= _BV(ISC01);	// Falling edge on int0
	sei();

	for( ;; );
}
