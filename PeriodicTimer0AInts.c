// PeriodicTimer0AInts.c
// Runs on LM4F120/TM4C123
// Use Timer0A in periodic mode to request interrupts at a particular
// period.
// Daniel Valvano
// September 11, 2013

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2015
  Program 7.5, example 7.6

 Copyright 2015 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

// oscilloscope or LED connected to PF3-1 for period measurement
// When using the color wheel, the blue LED on PF2 is on for four
// consecutive interrupts then off for four consecutive interrupts.
// Blue is off for: dark, red, yellow, green
// Blue is on for: light blue, blue, purple, white
// Therefore, the frequency of the pulse measured on PF2 is 1/8 of
// the frequency of the Timer0A interrupts.
//1. Timer, software start, analog comparator	
//2. Bit 3 of ADC0_RIS_R will be set when the conversion is on. this Bit triggers an interrupt
// 

//523 Hz*10= 5230 Hz; period = 15296 (C), 440*10 = 4400 Hz; period = 18181 (A) and 330*10 = 3300 Hz; period = 24242(E)

#include "../inc/tm4c123gh6pm.h"
#include <stdint.h>
#include "../inc/PLL.h"
#include "../inc/Timer0A.h"
#include "../inc/CortexM.h"
#include "../inc/LaunchPad.h"
#include "DAC.h"
#include "Switch.h"
#include "ST7735.h"
#include "Music1.h"
#include "Timer1A.h"
#include "SysTick.h" 
#include "ADCT0ATrigger.h"
  
#define WHEELSIZE 8           // must be an integer power of 2
                              //    red, yellow,    green, light blue, blue, purple,   white,          dark
const uint32_t COLORWHEEL[WHEELSIZE] = {RED, RED+GREEN, GREEN, GREEN+BLUE, BLUE, BLUE+RED, RED+GREEN+BLUE, 0};
uint32_t i = 0;
uint32_t ADC_VALUE = 0;
uint16_t Mode = 3;
uint32_t rate = 15296;
void UserTask(void){
  static uint32_t i = 0;
  LaunchPad_Output(COLORWHEEL[i&(WHEELSIZE-1)]);
  i = i + 1;
}

void audioProcessing(uint32_t data){
	PF3 ^= 0x08;           // toggle LED
  ADC_VALUE = data;
	uint32_t X_AXIS = 0;
	switch(Mode){
	
		case 1:
				DAC_Out(data);
			break;
    case 2:
				//streaming buffer
			break;
    case 3:
			//dac(SineWave)
			break;
	
	}
	
  ST7735_PlotClear(0,1500); 
  for(X_AXIS=0; X_AXIS<= 4095; X_AXIS++) {
     ST7735_DrawPixel((ADC_VALUE*128)/4096, 159-((X_AXIS*128)/4096), ST7735_BLUE);
  } 
	
}


// if desired interrupt frequency is f, Timer0A_Init parameter is busfrequency/f
#define F2HZ (80000000/2)
#define F20KHZ (80000000/20000)
//debug code
int main(void){
  PLL_Init(Bus80MHz);              // bus clock at 80 MHz
  LaunchPad_Init();                // activate port F
	ST7735_InitR(INITR_REDTAB);
	ST7735_OutString("Lab 9\n");
  ST7735_OutString("Audio Processing\n");
	ST7735_OutString("Frequency: 523	Hz\n");
	ADC0_InitTimer0ATriggerSeq0(1, rate,&audioProcessing); // ADC channel 0, 550 Hz sampling
	PF2^= 0x02;
	DAC_Init((2048<<1)); 
	NextNote();
	SysTick_Init();
  EnableInterrupts();
 
  while(1){
		//WaitForInterrupt();
		if(NVIC_ST_CURRENT_R <= 0x00FAFFFF){
			SongHandler();
			NVIC_ST_CURRENT_R = 0;
		}
  }
}


