/*
 * Jonathan Kaneshiro jkane002@ucr.edu
 * Andrew Cruz acruz029@ucr.edu
 * Lab Section: 24
 * Assignment: Lab 3 Exercise 3
 * Exercise Description: A car has a fuel-level sensor that sets PA3..PA0 to a value between 0 (empty) and 15 (full). 
 A series of LEDs connected to PC5..PC0 should light to graphically indicate the fuel level. 
 If the fuel level is 1 or 2, PC5 lights. If the level is 3 or 4, PC5 and PC4 light. 
 Level 5-6 lights PC5..PC3. 7-9 lights PC5..PC2. 10-12 lights PC5..PC1. 13-15 lights PC5..PC0. 
 Also, PC6 connects to a "Low fuel" icon, which should light if the level is 4 or less.  
 (The example below shows the display for a fuel level of 3).   
 
 In addition to the above, PA4 is 1 if a key is in the ignition, PA5 is one if a driver is seated, 
 and PA6 is 1 if the driver's seatbelt is fastened. PC7 should light a "Fasten seatbelt" icon
  if a key is in the ignition, the driver is seated, but the belt is not fastened. 
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
	DDRC = 0xFF; PORTC = 0x00;
	
	unsigned char tempA = 0x00;
	unsigned char tempC;
	unsigned char temp3 = 0x00;
    while (1) 
    {
		tempA = PINA & 0x0F;
		tempC = 0x00;
		temp3 = PINA;
		
		if(tempA <=2) {
			tempC = tempC | 0x60; // 0110 0000
		} else if(tempA <=4) {
			tempC = tempC | 0x70; // 0111 0000
		} else if(tempA <=6) {
			tempC = tempC | 0x38; // 0011 1000
		} else if(tempA <= 9) {
			tempC = tempC | 0x3C; // 0011 1100
		} else if(tempA <=12) {
			tempC = tempC | 0x3E; // 0011 1110
		} else if(tempA <=15) {
			tempC = tempC | 0x3F; // 0011 1111
		}
		
		//  PA4 is 1 if a key is in the ignition, PA5 is one if a driver is seated,
		// and PA6 is 1 if the driver's seatbelt is fastened.
		if(GetBit(temp3, 4) == 1 && GetBit(temp3, 5) == 1 && GetBit(temp3, 6) != 1) {
			tempC = SetBit(tempC, 7, 1); // PC7 should light a "Fasten seatbelt" icon
		}
		
		PORTC = tempC;
    }
}
