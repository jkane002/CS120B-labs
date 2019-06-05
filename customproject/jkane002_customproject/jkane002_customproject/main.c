#include <avr/io.h>
#include "usart.h"
#include "timer.c"
#include "io.c"
#include <stdio.h>
#include <stdlib.h>

enum states {init, buttonPress}  state;
unsigned char sendToArduino = 0x0000;
unsigned char tempB = 0x00;

unsigned char testOne = 0x00;
	
void Tick() {
	unsigned char A0 = ~PINA & 0x01;
	//unsigned char A1 = ~PINA>>1 & 0x01;
	
	switch(state) {
		case init:
			if(!A0) {
				state = init;
			} else if(A0) {
				state = buttonPress;
			}
				break;
		case buttonPress:
			if(A0)
				state = buttonPress;
			else if(!A0)
				state = init;
			break;
		default:
			state = init;
			break;
	}
	
	switch(state) {
		case init:
			tempB = 0x00;
			while (!USART_IsSendReady(0));
			USART_Send(0, 0);
			break;
		case buttonPress:
			//if(!USART_IsSendReady(0)) {
			tempB = 0x01;
			testOne = 0x0001;
			while (!USART_IsSendReady(0)) {};
			USART_Send(testOne, 0);

			//}
			//sendToArduino = 0x00FF;
			break;
		default:
			break;
	}
	
	PORTB = tempB;
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	TimerSet(100);
	TimerOn();
	 initUSART(0); // initializes USART0
	 USART_Flush(0);
	 
	 state = init;
	 
	 while(1){
		 Tick();
			//while (!USART_IsSendReady(0));
			USART_Send(sendToArduino & 0x0F, 0);
			//while(!USART_HasTransmitted(0));
			
			while (!TimerFlag) {} // Wait for timer period
			TimerFlag = 0; // Lower flag raised by timer
	 }
}