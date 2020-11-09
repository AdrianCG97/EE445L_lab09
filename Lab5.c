// SwitchTestMain.c
// Runs on LM4F120/TM4C123
// Test the switch initialization functions by setting the LED
// color according to the status of the switches.
// Daniel and Jonathan Valvano
// Jan 2, 2020

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2020
   Example 2.3, Program 2.9, Figure 2.36
 Copyright 2020 by Jonathan W. Valvano, valvano@mail.utexas.edu
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
#include <stdio.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/SwitchExternal.h"
#include "inc/Pll.h"
#include "inc/LaunchPad.h"
#include "inc/Systick.h"
#include "inc/ST7735.h"
#include "inc/fixed.h"
#include "inc/DAC.h"
#include "inc/Sound.h"
#include "inc/Timer0A.h"
#include "inc/Timer2A.h"


uint32_t soundAlarm = 0;
uint32_t updateTime = 0;

extern const unsigned char Paddle[];
	
enum timeAttribute{hours, minutes, seconds, timeOfDay, timeZone, hoursAlarm, minutesAlarm, secondsAlarm, timeOfDayAlarm, statusAlarm}; 

void Clock(void) {
	
	//Sound Alarm
	if(soundAlarm){
		Timer0A_Init(&Play_Sound, 80000000/11025, 1);
	}

	// Heartbeat
	PF2 ^= 04;
	
	// Update Time
	updateTime = 1;
	updateTimeSeconds();
}

int main(void){
	
	PLL_Init(Bus80MHz);     // 80 MHz
  LaunchPad_Init();       // initialize PF0 and PF4 and make them input; make PF3-1 out (PF3-1 built-in LEDs)
	displayInit();
	DAC_Init();
	enum timeAttribute currAttribute; 
	
	// Intialize Clock Interrupt
	Timer2A_Init(&Clock, 80000000, 2);
	
  while(1){
		
		uint32_t status = LaunchPad_Input();

		if(updateTime){
			changeClockFace();
			changeDisplayTimeHours();
			changeDisplayTimeMinutes();
			changeDisplayTimeSeconds();
			updateTime = 0;
		}

		if(isAlarmOn()){
			if(isItTime())
				soundAlarm = 1;
		}
		
		if(soundAlarm && (status == 0x02)){
				soundAlarm=0;
				Timer0A_Stop();
		}
		
		// Mode button pressed? Enter change mode.
    else if(status == 0x02){
			
			Timer2A_Stop();
			
			while(LaunchPad_Input() == 0x02){} // wait for release
			currAttribute = hours;

			// Change Mode
			while(1){
				LaunchPad_WaitForTouch();
				uint32_t mode = LaunchPad_Input();
				LaunchPad_WaitForRelease();

				// Done Changing Time?
				if(currAttribute > statusAlarm) {
					Timer2A_Init(&Clock, 80000000, 2);
					break;
				}
				// Next Attribute
				else if(mode == 0x02){
					currAttribute++;
				}
				// Up Arrow?
				else if(mode == 0x01){
					if(currAttribute == hours){
						updateTimeHours(5);
						changeDisplayTimeHours();
					}
					else if(currAttribute == minutes){
						updateTimeMinutes(1);
						changeDisplayTimeMinutes();
					}
					else if(currAttribute == seconds){
						updateTimeSeconds();
						changeDisplayTimeSeconds();
					}
					else if(currAttribute == timeOfDay){
						updateTimeAMPM();
						changeDisplayTimeAMPM();
					}
					else if(currAttribute == timeZone){
						updateTimeZone();
						changeTimeZone();
					}
					else if(currAttribute == hoursAlarm){
						updateAlarmHours(5);
						changeDisplayAlarmHours();
					}
					else if(currAttribute == minutesAlarm){
						updateAlarmMinutes(1);
						changeDisplayAlarmMinutes();
					}
					else if(currAttribute == secondsAlarm){
						updateAlarmSeconds();
						changeDisplayAlarmSeconds();
					}
					else if(currAttribute == timeOfDayAlarm){
						updateAlarmAMPM();
						changeDisplayAlarmAMPM();
					}
					else if(currAttribute == statusAlarm){
						setAlarm();
					}
				}
			}	
		}
	}
}