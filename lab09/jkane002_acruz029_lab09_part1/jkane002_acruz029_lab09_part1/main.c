/*	Partner 1 Name & E-mail: Jonathan Kaneshiro jkane002@ucr.edu
 *	Partner 2 Name & E-mail: Andrew Cruz acruz029@ucr.edu
 *	Lab Section: 24
 *	Assignment: Lab #9  Exercise #1 
 *	Exercise Description: [optional - include for your own benefit]
 *		Using the ATmega1284’s PWM functionality, design a system that uses 
 three buttons to select one of three tones to be generated on the speaker. 
 When a button is pressed, the tone mapped to it is generated on the speaker.
 *	code, is my own original work.
 */

#include <avr/io.h>

void set_PWM(double frequency) {
	static double current_frequency; // Keeps track of the currently set frequency
	
	// Will only update the registers when the frequency changes, otherwise allows
	// music to play uninterrupted.
	if (frequency != current_frequency) {
		if (!frequency) { TCCR3B &= 0x08; } //stops timer/counter

		else { TCCR3B |= 0x03; } // resumes/continues timer/counter
		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) { OCR3A = 0xFFFF; }
		// prevents OCR0A from underflowing, using prescaler 64     // 31250 is largest frequency that will not result in underflow
		
		else if (frequency > 31250) { OCR3A = 0x0000; }

		// set OCR3A based on desired frequency
		else { OCR3A = (short)(8000000 / (128 * frequency)) - 1; }
		
		TCNT3 = 0; // resets counter
		current_frequency = frequency; // Updates the current frequency
	}
}

void PWM_on() {
	TCCR3A = (1 << COM3A0);
	// COM3A0: Toggle PB3 on compare match between counter and OCR0A
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	// WGM02: When counter (TCNT0) matches OCR0A, reset counter
	// CS01 & CS30: Set a prescaler of 64
	set_PWM(0);
}

void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}

enum Notes{C, D, E, Silent} Note;

void Tick() {
	unsigned char A0 = ~PINA & 0x01;
	unsigned char A1 = ~PINA>>1 & 0x01;
	unsigned char A2 = ~PINA>>2 & 0x01;
	double sound = 0;
	
	switch (Note) {
		case C: {
			if(A0 & !A1 & !A2) {
				Note = C;
				} else if(!A0 & A1 & !A2) {
				Note = D;
				} else if(!A0 & !A1 & A2) {
				Note = E;
				} else {
				Note = Silent;
			}
			sound = 261.63;
			break;
		}
		case D: {
			if(A0 & !A1 & !A2) {
				Note = C;
				} else if(!A0 & A1 & !A2) {
				Note = D;
				} else if(!A0 & !A1 & A2) {
				Note = E;
				} else {
				Note = Silent;
			}
			sound = 293.66;
			break;
		}
		case E: {
			if(A0 & !A1 & !A2) {
				Note = C;
				} else if(!A0 & A1 & !A2) {
				Note = D;
				} else if(!A0 & !A1 & A2) {
				Note = E;
				} else {
				Note = Silent;
			}
			sound = 329.63;
			break;
		}
		case Silent: {
			if(A0 & !A1 & !A2) {
				Note = C;
				} else if(!A0 & A1 & !A2) {
				Note = D;
				} else if(!A0 & !A1 & A2) {
				Note = E;
				} else {
				Note = Silent;
			}
			sound = 0;
			break;
		}
		default: {
			sound = 392.00;
			break;
		}
	}
	
	set_PWM(sound);
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	
	PWM_on();
	Note = Silent;
	/* Replace with your application code */
	while (1)
	{
		Tick();
	}
}