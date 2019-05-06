/*	Partner 1 Name & E-mail: Jonathan Kaneshiro jkane002@ucr.edu
 *	Partner 2 Name & E-mail: Andrew Cruz acruz029@ucr.edu
 *	Lab Section: 24
 *	Assignment: Lab #8  Exercise #3 
 *	Exercise Description: [optional - include for your own benefit]
 *		Design a system where an LED is illuminated if enough light is detected from the photo resistor.
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
	unsigned char max = 0x38;
	ADC_init();
	
    /* Replace with your application code */
    while (1)
    {
		//unsigned short my_short = ADC;  // Value of ADC register now stored in variable x. 0x 0000 0011 1111 1111
		
		if(ADC >= max/2) {
			PORTB = 0x01;
		} else {
			PORTB = 0x00;
		}
    }
}
