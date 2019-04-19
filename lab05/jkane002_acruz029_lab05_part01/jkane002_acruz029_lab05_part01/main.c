/*	Partner 1 Name & E-mail: Jonathan Kaneshiro jkane002@ucr.edu
 *	Partner 2 Name & E-mail: Andrew Cruz acruz029@ucr.edu
 *	Lab Section: 24
 *	Assignment: Lab #5  Exercise # 1
 *	Exercise Description: [optional - include for your own benefit]
 *		(From an earlier lab) A car has a fuel-level sensor that sets PA3..PA0 to a value between 0 (empty) and 15 (full). 
 *		A series of LEDs connected to PB5..PB0 should light to graphically indicate the fuel level. 
 *		If the fuel level is 1 or 2, PB5 lights. 
 *		If the level is 3 or 4, PB5 and PB4 light. 
 *		5-6 lights PB5..PB3. 
 *		7-9 lights PB5..PB2. 
 *		10-12 lights PB5..PB1. 
 *		13-15 lights PB5..PB0. 
 *		Also, PB6 connects to a "Low fuel" icon, which should light if the level is 4 or less. 
 *		Use buttons on PA3..PA0 and mimic the fuel-level sensor with presses.
 *
 *		No press is 0
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>


int main(void)
{
	//init input
	DDRA = 0x00; PORTA = 0xFF;
	//init output
	DDRB = 0xFF; PORTB = 0x00;
	
	unsigned char tempA = 0x00;
	unsigned char tempB;
	while (1)
	{
		tempB = 0x00;
		tempA = PINA & 0x0F;
		tempA = ~tempA & 0x0F;
	
		if(tempA > 0) {
			if(tempA <= 2) { //If fuel is between 1-2 light up PC6-PC5
				tempB = tempB | 0x20;
				tempB = tempB | 0x40;
				} else if(tempA <= 4) { //If fuel is between 3-4 light up PC6-PC4
				tempB = tempB | 0x30;
				tempB = tempB | 0x40;
				} else if(tempA <= 6) { //If fuel is between 5-6 light up PC5-PC3
				tempB = tempB | 0x38;
				} else if(tempA <= 9) { //If fuel is between 7-9 light up PC5-PC2
				tempB = tempB | 0x3C;
				} else if(tempA <= 12) { //If fuel is between 10-12 light up PC5-PC1
				tempB = tempB | 0x3E;
				} else if(tempA <= 15) { //If fuel is between 13-15 light up PC5-PC0
				tempB = tempB | 0x3F;
			}
		}
		
		//Set PORTB to light up
		PORTB = tempB;
	}
}
