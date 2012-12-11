/* LED Blink Example with USB Debug Channel for Teensy USB Development Board
 * http://www.pjrc.com/teensy/
 * Copyright (c) 2008, 2010 PJRC.COM, LLC
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <util/delay.h>
#include "usb_rawhid.h"

// Teensy 2.0: LED is active high
#if defined(__AVR_ATmega32U4__) || defined(__AVR_AT90USB1286__)
#define LED_ON		(PORTD |= (1<<6))
#define LED_OFF		(PORTD &= ~(1<<6))

// Teensy 1.0: LED is active low
#else
#define LED_ON	(PORTD &= ~(1<<6))
#define LED_OFF	(PORTD |= (1<<6))
#endif

#define LED_CONFIG	(DDRD |= (1<<6))
#define CPU_PRESCALE(n)	(CLKPR = 0x80, CLKPR = (n))

int main(void)
{
	int8_t r;
	int32_t x = 0;
	uint8_t buffer[64];
	for (int i=0; i < 64; ++i) {
		buffer[i] = 0;
	}

	// set for 16 MHz clock, and make sure the LED is off
	CPU_PRESCALE(0);
	LED_CONFIG;
	LED_ON;
	
	// Configure inputs to be pull up
	DDRB &= ~((1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4));
	PORTB |= ((1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4));
	DDRE &= ~((1<<6) | (1<<7));
	PORTE |= ((1<<6) | (1<<7));


	usb_init();

	//Wait for usb to be configured
	while (!usb_configured());

	// blink morse code messages!
	while (1) {
		bool t0 = !(PINB & (1<<3));
		bool t1 = !(PINB & (1<<0));
		bool t2 = !(PINE & (1<<6));
		bool t3 = !(PINE & (1<<7));
		
		bool b0 = !(PINB & (1<<2));
		bool b1 = !(PINB & (1<<4));
		bool b2 = !(PINB & (1<<1));

		if (t0) {
			if (t2) {
				if (t3) {
					if (b0) {
						if (b1) {
							if (b2) {
								x = -9; //D
							} else {
								x = -7; //E
							}
						} else {
							x = -6; //F
						}
					} else {
						if (b1) {
							x = -5; //F#
						} else {
							x = -4; //G
						}
					}
				} else {
					x = -2; //A
				}
			} else {
				if (t1) {
					x = -1; //Bb
				} else {
					if (b0) {
						x = -1; //Bb
					} else {
						x = 0; //B
					}
				}
			}
		} else {
			if (t2) {
				x = 1; // C 
			} else {
				x = 2; // C#
			}
		}

		//Test shit
		if (!(PINB & (1<<0))) {
			LED_ON;
		} else {
			LED_OFF;
		}

		r = usb_rawhid_recv(buffer, 0);
		if (r > 0) { //Send a packet back
			memcpy(buffer, &x, sizeof(int32_t));
			usb_rawhid_send(buffer, 0);
		}
		_delay_ms(2);
	}
}
