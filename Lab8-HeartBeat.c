/*
* Rochester Institute of Technology
* Department of Computer Engineering
* CMPE 460  Interfacing Digital Electronics
* LJBeato
* 1/14/2021
*
* Filename: main_timer_template.c
*/
#include <stdio.h>
#include <stdlib.h>

#include "msp.h"
#include "uart.h"
#include "Timer32.h"
#include "CortexM.h"
#include "Common.h"
#include "ADC14.h"
// The sprintf function seemed to cause a change in the interrupt service routine.
// I think if we increase the HEAP size, it will work
// change to Heap_Size       EQU     0x00000200 in startup_msp432p401r_uvision.s

#define THRESHOLD 10000
BOOLEAN reading = FALSE;
unsigned long MillisecondCounter = 0;
#define point 1


// Interrupt Service Routine for Timer32-1
void Timer32_1_ISR(void)
{		
	char str[78];
	int beat = 0;
    int ch = 0;
	double freq = 0;
	int averager [point] = {0};
	ch = ADC_In();
	int sum = 0;
	double average = 0;
	for (int i = (point-1); i>0; i--){
		averager[i] = averager[i-1];
	}
	averager[0] = ch;
	for (int i = 0; i<point; i++){
		sum += averager[i];
	}
	average = (double)sum/point;
	//sprintf(str, "%f\r\n", average);
	//uart0_put(str);
	if (average>= THRESHOLD){
		if(!reading){
			TIMER32_CONTROL2 &= ~BIT7;
			freq = 1.0/(MillisecondCounter/1000.0);
			beat = freq*60;
			uart0_put("\r\nThe Heart Beat is:");
			sprintf(str, "%d BPM\r\n", beat);
			uart0_put(str);
			MillisecondCounter = 0;
			TIMER32_CONTROL2 |= BIT7;
			reading = TRUE;
		}
	}
	else{
		reading = FALSE;
	}
}


// Interrupt Service Routine
void Timer32_2_ISR(void)
{
	MillisecondCounter++;
}
// main
int main(void)
{
    //initializations
    uart0_init();
    uart0_put("\r\nLab8 Heart Beat demo\r\n");
    ADC0_InitSWTriggerCh6();
    Timer32_1_Init(&Timer32_1_ISR, SystemCoreClock/1000, T32DIV1);
    Timer32_2_Init(&Timer32_2_ISR, SystemCoreClock /1000, T32DIV1);
    EnableInterrupts();
    TIMER32_CONTROL1 |= BIT7;
	while(1){
		
		WaitForInterrupt();
	}
}
