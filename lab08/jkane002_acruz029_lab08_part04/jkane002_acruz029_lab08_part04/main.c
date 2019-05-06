/*	Partner 1 Name & E-mail: Jonathan Kaneshiro jkane002@ucr.edu
 *	Partner 2 Name & E-mail: Andrew Cruz acruz029@ucr.edu
 *	Lab Section: 24
 *	Assignment: Lab #8  Exercise #4 
 *	Exercise Description: [optional - include for your own benefit]
 *		Design a system, using a bank of eight LEDs, where the number of LEDs illuminated is a representation of how much light is detected.
 *		For example, when more light is detected, more LEDs are illuminated.
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
	
	unsigned char max = 0x7F;
	unsigned char slice = max/8;
	
	ADC_init();
	
    /* Replace with your application code */
    while (1)
    {
		unsigned short my_short = ADC;  // Value of ADC register
		
		if(ADC >= max) {
			PORTB = 0xFF;
		} else if (ADC >= (max - slice)) {
			PORTB = 0x7F;
		} else if(ADC >= (max - (2 * slice)) ) {
			PORTB = 0x3F;
		} else if(ADC >= (max - (3 * slice))) {
			PORTB = 0x1F;
		} else if(ADC >= (max - (4 * slice))) {
			PORTB = 0x0F;
		} else if(ADC >= (max - (5 * slice))) {
			PORTB = 0x07;
		} else if(ADC >= (max - (6 * slice))) {
			PORTB = 0x03;
		} else if(ADC >= (max - (7 * slice))) {
			PORTB = 0x01;
		} else if(ADC >= (max - (8 * slice)) + (slice/2)) {
			PORTB = 0x00;
		}		
    }
}
