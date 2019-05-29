#include <avr/io.h>
#include "usart.h"
#include "io.c"
#include "adc.c"
#include <stdio.h>
#include <stdlib.h>

enum states {init, one, two, three}  state;
	
void Tick() {
	switch(state) {
		case init:
		break;
		case one:
		break;
		case two:
		break;
		case three:
		break;
		default:
		break;
	}
}

int main(void)
{
	 
	 DDRA= 0x00; PORTA = 0xFF;
	 
	 initUSART(0); // initializes USART0
	 USART_Flush(0);
	 state = init;
	 while(1){
		 
	 }
}