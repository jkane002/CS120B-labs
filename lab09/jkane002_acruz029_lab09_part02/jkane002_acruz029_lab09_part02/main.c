/*	Partner 1 Name & E-mail: Jonathan Kaneshiro jkane002@ucr.edu
 *	Partner 2 Name & E-mail: Andrew Cruz acruz029@ucr.edu
 *	Lab Section: 24
 *	Assignment: Lab #9  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *		Using the ATmega1284’s PWM functionality, design a system where the notes: C4, D, E, F, G, A, B, and C5,  
 * from the table at the top of the lab, can be generated on the speaker by scaling up or down the eight note scale. 
 * Three buttons are used to control the system. One button toggles sound on/off. 
 * The other two buttons scale up, or down, the eight note scale. 
 * 
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>

// 0.954 hz is lowest frequency possible with this function,
// based on settings in PWM_on()
// Passing in 0 as the frequency will stop the speaker from generating sound
void set_PWM(double frequency) {
	static double current_frequency; // Keeps track of the currently set frequency
	// Will only update the registers when the frequency changes, otherwise allows
	// music to play uninterrupted.
	if (frequency != current_frequency) {
		if (!frequency) { TCCR0B &= 0x08; } //stops timer/counter
		else { TCCR0B |= 0x03; } // resumes/continues timer/counter
		
		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) { OCR0A = 0xFFFF; }
		
		// prevents OCR0A from underflowing, using prescaler 64					// 31250 is largest frequency that will not result in underflow
		else if (frequency > 31250) { OCR0A = 0x0000; }
		
		// set OCR3A based on desired frequency
		else { OCR0A = (short)(8000000 / (128 * frequency)) - 1; }

		TCNT0 = 0; // resets counter
		current_frequency = frequency; // Updates the current frequency
	}
}

void PWM_on() {
	TCCR0A = (1 << WGM02) | (1 << WGM00) | (1 << COM0A0);
	// COM3A0: Toggle PB3 on compare match between counter and OCR0A
	TCCR0B = (1 << WGM02) | (1 << CS01) | (1 << CS00);
	// WGM02: When counter (TCNT0) matches OCR0A, reset counter
	// CS01 & CS30: Set a prescaler of 64
	set_PWM(0);
}

void PWM_off() {
	TCCR0A = 0x00;
	TCCR0B = 0x00;
}


enum sound_states{Init, Inc, Dec, Start, Wait, Press}sound_state;

unsigned char onoff = 0x00;
unsigned char low = 0x00;
unsigned char high = 0x07;
unsigned char place = 0x00;
unsigned char substitute = 0x00;

double notes[8] = {261.63, 293.66, 329.63, 349.23, 392, 440, 493.88, 523.25};
double temp;

void Tick_fn() {
	switch(sound_state){
		case Init: // init state
			sound_state = Start;
			break;
		
		case Start:
		if((~PINA & 0x07) == 0x01){ // first button increments
			sound_state = Inc;
			break;
		}
		else if((~PINA & 0x07) == 0x02){ // second button decrements
			sound_state = Dec;
			break;
		}
		else if((~PINA & 0x07) == 0x04){ // on/off
			sound_state = Press;
			break;
		}
		else{
			sound_state = Start;
			break;
		}
		
		case Inc:
			temp = notes[place];
			sound_state = Wait;
		break;
		
		case Dec:
		temp = notes[place];
		sound_state = Wait;
		break;
		
		case Wait:
		if((~PINA & 0x07) == 0x00){
			sound_state = Init;
			break;
		}
		else{
			sound_state = Wait;
			break;
		}
		
		case Press:
		sound_state = Wait;
		break;
		
		default:
		sound_state = Init;
		break;
	}
	
	switch(sound_state){ //actions
		case Init:
		break;
		case Start:
		break;
		
		case Inc:
		if((place + 1) > high){ // remains at the high value
			place = high;
			break;
		}
		else{	//increases the note
			++place;
			break;
		}
		
		case Dec:
		if((place-1) < low){ //remains at the lowest val
			place = low;
			break;
		}
		else{ // decreases the note
			--place;
			break;
		}
		case Wait:
		temp = notes[place];
		set_PWM(temp);
		break;
		
		case Press:
		if(onoff){ // if 0, then off
			PWM_off();
			substitute = place;
			place = 8;
			onoff = 0;
			break;
		}
		else{ // if 1, then on
			PWM_on();
			place = substitute;
			onoff = 1;
			break;
		}
		default:
		break;
	}
	
}

int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	/* Replace with your application code */
	
	sound_state = Init;
	PWM_on();
	onoff = 1; // 1 is on automatically

    while (1) 
    {
		Tick_fn();
    }
}

