#include <avr/io.h>
#include "usart.h"
#include "timer.c"
#include "io.c"
#include <stdio.h>
#include <stdlib.h>

unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

enum states {init, bubble, insert}  state;
//enum states {init, buttonPress, buttonWait}  state;

unsigned char sendToArduino = 0x0000;
unsigned char tempB = 0x00;
unsigned char START_BUBBLE = 0x00;

unsigned char testOne = 0x00;
	
void Tick() {
	unsigned char A0 = GetBit(~PINA, 0);
	unsigned char A1 = ~PINA>>1 & 0x01;

	unsigned char A0_pressed = A0 && !A1;
	unsigned char A1_pressed = !A0 && A1;
	unsigned char nothing_pressed = !A0 && !A1;

	
	switch(state) {
		case init:
			if(nothing_pressed) {
				state = init;
			} else if(A0_pressed) {
				state = bubble;
			} else if (A1_pressed) {
				state = insert;
			} else {
				state = init;
			}
				break;
		case bubble:
			if(nothing_pressed) {
				state = init;
				} else if(A0_pressed) {
				state = bubble;
				} else if (A1_pressed) {
				state = insert;
				} else {
				state = init;
			}
			break;
		case insert:
		if(nothing_pressed) {
			state = init;
			} else if(A0_pressed) {
			state = bubble;
			} else if (A1_pressed) {
			state = insert;
			} else {
			state = init;
		}
			break;
		default:
			state = init;
			break;
	}
	
	switch(state) {
		case init:
			tempB = 0x00;
//			LCD_ClearScreen();
	//		LCD_DisplayString(1, "Press a button");
			while (!USART_IsSendReady(0));
			USART_Send(0, 0);
			break;
		case bubble:
			//if(!USART_IsSendReady(0)) {
			tempB = 0x01;
			testOne = 0x0001;
			
			while (!USART_IsSendReady(0)) {};
			USART_Send(testOne, 0);
			//LCD_ClearScreen();
			//LCD_DisplayString(1,"Bubble Sort");
			//}
			break;
			
			case insert:
			tempB = 0x02;
			//testTwo = 0x0002;
			
			while (!USART_IsSendReady(0)) {};
			USART_Send(0x0002, 0);
			break;
		default:
			break;
	}
	
	
/*
	switch(state) {
		case Button_Wait:{
			if(A0) {
				state = Button_Press;
			} else {
				state = Button_Wait;
			}
			break;
		case Button_Press: 
			state = Button_Release;
			break;
		case Button_Release: 
			if(!A0) {
				state = Button_Wait;
			} else {
				state = Button_Release;
			}
			break;
		default:
			state = Button_Wait;
			break;
		}
		
		switch(state) {
			case Button_Wait:
				tempB = 0x00;
				//while (!USART_IsSendReady(0));
				//USART_Send(0, 0);
				break;
			case Button_Press:
				//START_BUBBLE = (START_BUBBLE == 0) ? 1 : 0;
				tempB = 0x01;
				////	while (!USART_IsSendReady(0)) {};
					//USART_Send(0x01, 0);
				break;
			case Button_Release: 
				break;
			default: 
				break;
		}
		
	}
	*/
	
	PORTB = tempB;
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	TimerSet(200);
	TimerOn();
	initUSART(0); // initializes USART0
	USART_Flush(0);
	 
	 //LCD_init();
	 state = init;
	 //state = Button_Wait;
	 			
	 while(1){
		 Tick();
			
		while (!TimerFlag) {} // Wait for timer period
		TimerFlag = 0; // Lower flag raised by timer
	 }
}