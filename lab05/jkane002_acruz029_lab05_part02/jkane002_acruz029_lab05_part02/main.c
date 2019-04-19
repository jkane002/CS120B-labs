/*	Partner 1 Name & E-mail: Andrew Cruz acruz029@ucr.edu
 *	Partner 2 Name & E-mail: Jonathan Kaneshiro jkane002@ucr.edu
 *	Lab Section: 24
 *	Assignment: Lab # 5  Exercise # 2 
 *	Exercise Description: [optional - include for your own benefit]
 *		(From an earlier lab) Buttons are connected to PA0 and PA1. 
 *		Output for PORTB is initially 0. 
 *		Pressing PA0 increments PORTB (stopping at 9). 
 *		Pressing PA1 decrements PORTB (stopping at 0). 
 *		If both buttons are depressed (even if not initially simultaneously), PORTB resets to 0. 
 *		If a reset occurs, both buttons should be fully released before additional increments or decrements are allowed to happen. 
 *		Use LEDs (and resistors) on PORTB. 
 *		Use a state machine (not synchronous) captured in C.
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
 
#include <avr/io.h>


unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}



enum Num_States {Wait, Increment, Decrement, Reset} Num_State;

void State_Machine() {
	unsigned char PA_01 = PINA & 0x03;
	PA_01 = ~PA_01 & 0x03;
	unsigned char tempB = PORTB;
	
	switch(Num_State) {
		case Wait: {
			if(PA_01 == 0x01) { //Increment PA1 = 0 PA0 = 1
				Num_State = Increment;
				tempB = (tempB >= 0x09) ? 0x09 : tempB + 0x01;
			} else if(PA_01 == 0x02) { //Decrement PA1 = 1 PA0 = 0
				Num_State = Decrement;
				tempB = (tempB <= 0x00) ? 0x00 : tempB - 0x01;
			} else if(PA_01 == 0x03) { //Reset PA1 = 1 PA0 = 1
				Num_State = Reset;
				tempB = 0x00;
			} else { //Anything else just wait 
				Num_State = Wait;
			}
			break;
		}
		case Increment: {
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
			if(PA_01 == 0x02) { //If still prseed down wait in this state
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
			tempB = 0x00;
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
			break;
		}
	}
	PORTB = tempB;
}


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x07;
	Num_State = Wait;
	
	while (1)
	{
		State_Machine();
		
	}
}

