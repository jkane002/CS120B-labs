/*
* Jonathan Kaneshiro jkane002@ucr.edu
* Andrew Cruz acruz029@ucr.edu
* Lab Section: 24
* Assignment: Lab 3 Exercise 4
* Exercise Description:
(Challenge): Read an 8-bit value on PA7..PA0 and write that value on PB3..PB0PC7..PC4. 
That is to say,  take the upper nibble of PINA and map it to the lower nibble of PORTB, 
likewise take the lower nibble of PINA and map it to the upper nibble of PORTC (PA7 -> PB3, PA6 -> PB2, … PA1 -> PC5, PA0 -> PC4).
*
* I acknowledge all content contained herein, excluding template or example code, is my own original work.
*/

#include <avr/io.h>

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}


int main(void)
{
    /* Replace with your application code */
	
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	
	unsigned char tempA = 0x00;
	unsigned char tempB = 0x00;
	unsigned char tempC = 0x00;
	
    while (1) 
    {
		tempA = PINA;
		
		// Sets upper half of tempA to lower half of tempB
		tempB = SetBit(tempB, 3, GetBit(tempA, 7));
		tempB = SetBit(tempB, 2, GetBit(tempA, 6));
		tempB = SetBit(tempB, 1, GetBit(tempA, 5));
		tempB = SetBit(tempB, 0, GetBit(tempA, 4));
		
		// Sets lower half of tempA to upper half of tempC 
		tempC = SetBit(tempC, 7, GetBit(tempA, 3));
		tempC = SetBit(tempC, 6, GetBit(tempA, 2));
		tempC = SetBit(tempC, 5, GetBit(tempA, 1));
		tempC = SetBit(tempC, 4, GetBit(tempA, 0));
		
		PORTB = tempB;
		PORTC = tempC;
		
    }
}

