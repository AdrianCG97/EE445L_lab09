// Music.h
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

#ifndef __Music1_H__ // do not include more than once
#define __Music1_H__
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "../inc/CortexM.h"
#include "SysTick.h"

struct MusicState{
	uint32_t Pitch0;		// freq = 375000/Pitch
	uint16_t Duration;	// number of 8.096ms intervals
};

extern uint16_t odeToJoy[32];
typedef const struct MusicState MusicStateType;
extern int32_t Position;			// index into SongTable
extern int32_t Play;					// true if song is playing
extern uint32_t  SongCount;	// counter for play current note

extern MusicStateType SongTable[78];

extern uint32_t I0;
extern const uint32_t *TheWave;
extern uint32_t Period0;
extern uint32_t Out0;

//struct VoiceState{
//	const unsigned char *VPt;
//	const struct VoiceState *next;
//	const uint16_t prescale;
//};
//typedef const struct VoiceState VoiceStateType;

//VoiceStateType Voices[1] = {
//	{odeToJoy, &Voices[1], 0}
//};
//VoiceStateType *WhichVoice = Voices;

//------------Timer_Arm-------------------
void Timer_Arm(void);

//------------Timer_Disarm-----------------
void Timer_Disarm(void);

void Rewind(void);

void NextNote(void);

void SongHandler(void);

#endif // __Music1_H__
	
