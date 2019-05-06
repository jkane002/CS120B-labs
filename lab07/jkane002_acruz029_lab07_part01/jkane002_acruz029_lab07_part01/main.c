/*	Partner 1 Name & E-mail: Andrew Cruz acruz029@ucr.edu
 *	Partner 2 Name & E-mail: Jonathan Kaneshiro jkane002@ucr.edu
 *	Lab Section: 24
 *	Assignment: Lab #7  Exercise #1 
 *	Exercise Description: [optional - include for your own benefit]
 *		Buttons are connected to PA0 and PA1. Output PORTC and PORTD drive the LCD display, initially displaying 0. 
 *		Pressing PA0 increments the display (stopping at 9). 
 *		Pressing PA1 decrements the display (stopping at 0). 
 *		If both buttons are depressed (even if not initially simultaneously), the display resets to 0. 
 *		If a button is held, then the display continues to increment (or decrement) at a rate of once per second. 
 *		Use a synchronous state machine captured in C
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include "io.c"

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

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}

unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

enum Num_States {Init,Wait, Increment, Decrement, Reset} Num_State;

unsigned char COUNT;

void State_Machine() {
	//Getting input
	unsigned char PA_01 = PINA & 0x03;
	PA_01 = ~PA_01 & 0x03;
	//Getting to output to C
	unsigned char tempC = PORTC;
	unsigned char tempB;
	//SM
	switch(Num_State) {
		case Init: {
			Num_State = Wait;
			tempC = '7';
			tempB = 0x00;
			COUNT = 0;
		}
		case Wait: {
			//Reset count to 0
			COUNT = 0;
			//Set LED to 1 for state 1
			tempB = 0x01;
			//Transitions
			if(PA_01 == 0x01) { //Increment PA1 = 0 PA0 = 1
				Num_State = Increment;
				//Increment LED Screen
				tempC = (tempC >= '9') ? '9' : tempC + 1;
			} else if(PA_01 == 0x02) { //Decrement PA1 = 1 PA0 = 0
				Num_State = Decrement;
				//Decrement LED SCreen
				tempC = (tempC <= '0') ? '0' : tempC - 1;
			} else if(PA_01 == 0x03) { //Reset PA1 = 1 PA0 = 1
				Num_State = Reset;
			} else { //Anything else just wait
				Num_State = Wait;
			}
			break;
		}
		case Increment: {
			//Increment Count
			COUNT++;
			//Check if button has been held for a second
			if(COUNT >= 8) {
				tempC = (tempC >= '9') ? '9' : tempC + 1;
				COUNT = 0;
			}
			//Set LED to 2
			tempB = 0x02;
			//Transitions
			if(PA_01 == 0x01) { //If button is still down stay in state
				Num_State = Increment;
			} else if(PA_01 == 0x00) { //If both button are released go waiting state
				Num_State = Wait;
			} else if(PA_01 == 0x03) { //If both buttons are down go to reset
				Num_State = Reset;
			} else { //Anything else
				Num_State = Wait;
			}
			break;
		}
		case Decrement: {
			//Increment Count
			COUNT++;
			//Check if button has been held for a second
			if(COUNT >= 8) {
				tempC = (tempC <= '0') ? '0' : tempC - 1;
				COUNT = 0;
			}
			//Set Lights to 3
			tempB = 0x03;
			//Transitions
			if(PA_01 == 0x02) { //If still pressed down wait in this state
				Num_State = Decrement;
			} else if(PA_01 == 0x00) { //If both buttons released then go to wait state
				Num_State = Wait;
			} else if(PA_01 == 0x03) { //If both buttons pressed go to reset
				Num_State = Reset;
			} else { //Else
				Num_State = Wait;
			}
			break;
		}
		case Reset: {
			//Reset display
			tempC = '0';
			//Set light to 0
			tempB = 0x04;
			//Transitions
			if(PA_01 == 0x03) { //Both Button still depressed
				Num_State = Reset;
			} else if (PA_01 == 0x00) { //If both buttons get depressed
				Num_State = Wait;
			} else { //Anything else goes to reset
				Num_State = Reset;
			}
			break;
		}
		default : { //Errors go to state Wait
			Num_State = Reset;
			tempB = 0x00;
			tempC = '0';
			break;
		}
	}
	LCD_ClearScreen();
	LCD_WriteData(tempC);
	PORTB = tempB;
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = '0' + 7; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines

	
	TimerSet(125);
	TimerOn();
	Num_State = Init;
	
	LCD_init();
	
	while(1) {
		State_Machine();
		while (!TimerFlag);
		TimerFlag = 0;
		// Note: For the above a better style would use a synchSM with TickSM()
		// This example just illustrates the use of the ISR and flag
	}
}