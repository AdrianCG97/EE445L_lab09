 // DAC.h
// Runs on LM4F120/TM4C123
// Use Timer0A in periodic mode to request interrupts at a particular
// period.
// Daniel Valvano
// May 2, 2015

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2014
  Program 7.5, example 7.6

 Copyright 2014 by Jonathan W. Valvano, valvano@mail.utexas.edu
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

 #include <stdint.h>
 #include "../inc/tm4c123gh6pm.h"
 #include "../inc/CortexM.h"
 
void DAC_Init(uint16_t data){
	SYSCTL_RCGCSSI_R |= 0x02;       // activate SSI1
	SYSCTL_RCGCGPIO_R |= 0x08;			//Enable clock for Port D
	while((SYSCTL_RCGCGPIO_R&0x08)==0);		//
	 // ---------------  Initialize PD2 as AIN5  ----------------------------------         
	GPIO_PORTD_DIR_R      &= ~0x04;           // make PD2 input   
	GPIO_PORTD_AFSEL_R    |=  0x04;           // enable alternate function on PD2   
	GPIO_PORTD_DEN_R      &= ~0x04;           // disable digital I/O on PD2   
	GPIO_PORTD_AMSEL_R    |=  0x04;           // enable analog functionality on PD2 
 
  // ---------------  Initialize PD3,1,0 as SSI1 MOSI, FS & SCK  ----------------         
	GPIO_PORTD_AMSEL_R    &= ~0x0B;           // disable analog functionality on PD   
	GPIO_PORTD_AFSEL_R    |=  0x0B;           // enable alt funct on PD3,1,0   
	GPIO_PORTD_DEN_R      |=  0x0B;           // enable digital I/O on PD3,1,0   
	GPIO_PORTD_PCTL_R      = (GPIO_PORTD_PCTL_R & 0xFFFF0F00) + 0x00002022;

	//
	SSI1_CPSR_R = 0x08;             // 80MHz/8 = 10 MHz SSIClk (should work up to 20 MHz)
	//SSI1_CPSR_R = 0x04;             // 80MHz/4 = 20 MHz SSIClk (should work up to 20 MHz)
  SSI1_CR0_R &= ~(0x0000FFF0);    // SCR = 0, SPH = 0, SPO = 1 Freescale
  SSI1_CR0_R += 0x40;             // SPO = 1
	//SSI1_CR0_R += 0x40;             // SPO = 1
  SSI1_CR0_R |= 0x0F;             // DSS = 16-bit data
	SSI1_DR_R = data;               // load 'data' into transmit FIFO
  SSI1_CR1_R |= 0x00000002;       // enable SSI

}

void DAC_Out(uint16_t data){
		GPIO_PORTD_DATA_R &= ~0x02;        // PD1 FSS low	
	  while((SSI1_SR_R&0x00000002)==0){};// wait until room in FIFO
		SSI1_DR_R = data; // data out
		 GPIO_PORTD_DATA_R |= 0x02;         // PD1 FSS high
}
