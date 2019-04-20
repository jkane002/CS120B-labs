/*	Partner 1 Name & E-mail: Jonathan Kaneshiro jkane002@ucr.edu
 *	Partner 2 Name & E-mail: Andrew Cruz acruz029@ucr.edu
 *	Lab Section: 24
 *	Assignment: Lab # 5  Exercise # 3 
 *	Exercise Description: [optional - include for your own benefit]
 *		Create your own festive lights display with 6 LEDs connected to port PB5..PB0, lighting in some attractive sequence. 
 *		Pressing the button on PA0 changes the lights to the next configuration in the sequence. 
 *		Use a state machine (not synchronous) captured in C.2^
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
 
#include <avr/io.h>
#define LEFT 0
#define RIGHT 1

enum ButtonPress{Init, Wait, ButtonDown}ButtonPressed;

unsigned char DIRECTION;

void SM_Sequence() {
	unsigned char A_0 = PINA & 0x01;
	A_0 = ~A_0 & 0x01; //Gets button press inverted
	
	unsigned char tempB = PORTB;

	switch (ButtonPressed) {
		case Init: { //Starting state
			ButtonPressed = Wait;
			tempB = 0x00;
			DIRECTION = LEFT;
			break;
		}
		case Wait: { //Waiting for button to be pressed
			if(A_0) { //Button is pressed
				ButtonPressed = ButtonDown;		//Move to next state
				
				if(tempB >= 0x7F) {	//Check if we need to go in other direction
					DIRECTION = RIGHT;
				} else if(tempB <= 0x00) { //Check to see if we need to go in other direction
					DIRECTION = LEFT;
				}
				
				if(DIRECTION == RIGHT) {	//Modify B according to direction
					tempB = tempB >> 1;
				} else if(DIRECTION == LEFT) {
					tempB = (tempB << 1) | 0x01;
				}
				
				PORTB = tempB;
			} else {	//Anything else go to wait state
				ButtonPressed = Wait;
			}
			break;
		}
		case ButtonDown: {	//Button is pressed state
			if(A_0) {	//Button is being held
				ButtonPressed = ButtonDown;
			} else {	//Button is released
				ButtonPressed = Wait;
			}
			break;
		}
		default: { //Errors go to init
			ButtonPressed = Init;
			break;
		}
	}
}

int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	ButtonPressed = Init;
    while (1) 
    {
		SM_Sequence();
    }
}
