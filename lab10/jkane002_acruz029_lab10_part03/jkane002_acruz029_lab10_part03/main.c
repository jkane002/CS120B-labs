/*	Partner 1 Name & E-mail: Jonathan Kaneshiro jkane002@ucr.edu
 *	Partner 2 Name & E-mail: Andrew Cruz acruz029@ucr.edu
 *	Lab Section: 24
 *	Assignment: Lab #10  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *	To the previous exercise's implementation, connect your speaker's red wire to PB4
	 and black wire to ground. Add a third task that toggles PB4 on for 2 ms and off 
	 for 2 ms as long as a switch on PA2 is in the on position.
 * 
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

void TimerOn() {
	// AVR timer/counter controller register TCCR1
	TCCR1B = 0x0B;// bit3 = 0: CTC mode (clear timer on compare)
	// bit2bit1bit0=011: pre-scaler /64
	// 00001011: 0x0B
	// SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
	// Thus, TCNT1 register will count at 125,000 ticks/s

	// AVR output compare register OCR1A.
	OCR1A = 125;	// Timer interrupt will be generated when TCNT1==OCR1A
	// We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
	// So when TCNT1 register equals 125,
	// 1 ms has passed. Thus, we compare to 125.
	// AVR timer interrupt mask register
	TIMSK1 = 0x02; // bit1: OCIE1A -- enables compare match interrupt

	//Initialize avr counter
	TCNT1=0;

	_avr_timer_cntcurr = _avr_timer_M;
	// TimerISR will be called every _avr_timer_cntcurr milliseconds

	//Enable global interrupts
	SREG |= 0x80; // 0x80: 1000000
}

void TimerOff() {
	TCCR1B = 0x00; // bit3bit1bit0=000: timer off
}

void TimerISR() {
	TimerFlag = 1;
}

// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect) {
	// CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) { // results in a more efficient compare
		TimerISR(); // Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

enum TL_States {TL_SMStart, TL_s1, TL_s2, TL_s3} TL_State;
enum BL_States {BL_SMStart, BL_s1} BL_State;
enum S_States {s_on, s_off} S_state;


//Shared Variables
unsigned char TEMP_THREELED;
unsigned char TEMP_BLINKLED;
unsigned char TEMP_SPEAKER;
unsigned char S_output;

int TickFct_ThreeLEDs(int state) {
	switch(state) { // Transitions
		case TL_SMStart: // Initial transition
		state = TL_s1;
		break;
		case TL_s1:
		state = TL_s2;
		break;
		case TL_s2:
		state = TL_s3;
		break;
		case TL_s3:
		state = TL_s1;
		break;
		default:
		state = TL_SMStart;
	} // Transitions

	switch(state) { // State actions
		case TL_s1:
		TEMP_THREELED = 0x01;
		break;
		case TL_s2:
		TEMP_THREELED = 0x02;
		break;
		case TL_s3:
		TEMP_THREELED = 0x04;
		break;
		default:
		break;
	} // State actions
	return state;
}

int TickFct_BlinkLED(int state) {
	switch(state) { // Transitions
		case BL_SMStart: // Initial transition
		TEMP_BLINKLED = 0; // Initialization behavior
		state = BL_s1;
		break;
		case BL_s1:
		state = BL_s1;
		break;
		default:
		state = BL_SMStart;
	} // Transitions

	switch(state) { // State actions
		case BL_s1:
		TEMP_BLINKLED = !TEMP_BLINKLED;
		break;
		default:
		break;
	} // State actions
	return state;
}

int speaker_tick(int state) {
	unsigned char button = ~PINA & 0x04;
	switch(state) {
		case s_on:
			state = s_off;
			break;
		case s_off:
			if(button) {
				state = s_on;
			} else {
				state = s_off;
			}
			break;
		default:
			state = s_on;
			break;
	}
	switch(S_state) {
		case s_on:
		TEMP_SPEAKER = 0x10;
		break;
		case s_off:
		TEMP_SPEAKER = 0;
		break;
	}
	
	return state;
}

int COUNT_BLINK;
int COUNT_THREE;
int COUNT_SPEAK;

void TickFct_Controller() {
	if(COUNT_BLINK >= 1000) {
		BL_State = TickFct_BlinkLED(BL_State);
		PORTB = TEMP_THREELED | (TEMP_BLINKLED << 3) | TEMP_SPEAKER;
		COUNT_BLINK = 0;
	}
	if(COUNT_THREE >= 300) {
		TL_State = TickFct_ThreeLEDs(TL_State);
		PORTB = TEMP_THREELED | (TEMP_BLINKLED << 3) | TEMP_SPEAKER;
		COUNT_THREE = 0;
	}
	
	if(COUNT_SPEAK >= 3) {
		S_state = speaker_tick(S_state);
		PORTB = TEMP_THREELED | (TEMP_BLINKLED << 3) | TEMP_SPEAKER;
		COUNT_SPEAK = 0;
	}
	
	COUNT_THREE++;
	COUNT_BLINK++;
	COUNT_SPEAK++;
}

int main(void)
{
	DDRB = 0xFF; PORTB = 0x00;
	DDRA = 0x00; PORTA = 0xFF;
	//Set to 1 second
	TimerSet(1);
	TimerOn();
	
	while(1) {
		TickFct_Controller();
		while(!TimerFlag);
		TimerFlag = 0;
	}
	return 0;
}
