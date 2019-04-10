/*	Partner 1 Name & E-mail: Jonathan Kaneshiro jkane002@ucr.edu
 *	Partner 2 Name & E-mail: Andrew Cruz acruz029@ucr.edu
 *	Lab Section: 24
 *	Assignment: Lab # 3  Exercise # 5 
 *	Exercise Description: [optional - include for your own benefit]
 *		(Challenge): A car's passenger-seat weight sensor outputs a 9-bit value (ranging from 0 to 511) 
 *		and connects to input PD7..PD0PB0 on the microcontroller. 
 *		If the weight is equal to or above 70 pounds, the airbag should be enabled by setting PB1 to 1. 
 *		If the weight is above 5 but below 70, the airbag should be disabled and an "Airbag disabled" icon should light by setting PB2 to 1. 
 *		(Neither B1 nor B2 should be set if the weight is 5 or less, as there is no passenger). 
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */


#include <avr/io.h>

// Bit-access function
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}


int main(void)
{
	//Init inputs and outputs
	DDRD = 0x00; PORTD = 0xFF;
	DDRB = 0x00; PORTB = 0xFF;
	//Stores weight
	unsigned char weight = 0x00;
	//Read Pins
	unsigned char tempD;
	unsigned char tempB;
	//Stores output for B
	unsigned char outB;
	
    /* Replace with your application code */
    while (1) 
    {
		tempB = PINB;
		tempD = PIND;
		outB = 0x00;
		
		if(GetBit(tempD,7)) {
			outB = SetBit(outB,1,1);
		} else {
			tempD = tempD << 1;
			weight = tempD + GetBit(tempB,0);
			
			if(weight >= 70) {
				outB = SetBit(outB,1,1);
			} else if (5 < weight && weight < 70) {
				outB = SetBit(outB,2,1);
			}
		}
		PORTB = outB; 
    }
}
