#include "msp.h"
#include "Common.h"

#define SW1 BIT1;
#define SW2 BIT4;

void Switch1_Init(void){
	// configure PortPin for Switch 1
	P1->SEL0 &= ~SW1;
	P1->SEL1 &= ~SW1;
	// configure as input
	P1->DIR &= ~SW1;
	// enable pullup resistor
	P1->REN |= SW1;
	P1->OUT |= SW1;
}

void Switch2_Init(void){
	// configure PortPin for Switch 2
	P1->SEL0 &= ~SW2;
	P1->SEL1 &= ~SW2;
	// configure as input
	P1->DIR &= ~SW2;
	// enable pullup resistor
	P1->REN |= SW2;
	P1->OUT |= SW2;
}
