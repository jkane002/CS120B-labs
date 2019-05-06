/*	Partner 1 Name & E-mail: Jonathan Kaneshiro jkane002@ucr.edu
 *	Partner 2 Name & E-mail: Andrew Cruz acruz029@ucr.edu
 *	Lab Section: 24
 *	Assignment: Lab #8  Exercise #1 and #2
 *	Exercise Description: [optional - include for your own benefit]
 *		Make sure your breadboard is wired according to the prelab. 
 *		The potentiometer is used to adjust the voltage supplied to the microcontroller for ADC . 
 *		Design a system that reads the 10-bit ADC result from the ADC register, and displays the result on a bank of 10 LEDs.
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	//        in Free Running Mode, a new conversion will trigger whenever
    //        the previous conversion completes.
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	ADC_init();
	
    /* Replace with your application code */
    while (1)
    {
		unsigned short my_short = ADC;  // Value of ADC register now stored in variable x. 0x 0000 0011 1111 1111
		
		unsigned char lower = (char)my_short; // my_char = 0xCD
		unsigned char upper = (char)(my_short >> 8); // my_char = 0xBC
		
		
		PORTB = lower;
		PORTD = upper;
    }
}
