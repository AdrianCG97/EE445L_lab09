
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
#include "Timer1A.h"
#include "Dac.h"
		
	
#define C0 305781	//9556   // 261.626 Hz			
#define DF 288618   // 277.183 Hz			
#define D 272419   // 293.665 Hz			
#define EF 257130   // 311.127 Hz			
#define E 242698   // 329.628 Hz			
#define F 229077   // 349.228 Hz			
#define GF 216219   // 369.994 Hz			
#define G 204084   // 391.995 Hz			
#define AF 192630   // 415.305 Hz			
#define A 181818   // 440.000 Hz			
#define BF 171614   // 466.164 Hz			
#define B 161982   // 493.883 Hz			
#define C 152890   // 523.251 Hz			

//#define C0 9556   // 261.626 Hz	
//#define DF 9019   // 277.183 Hz	
//#define D 8513   // 293.665 Hz	
//#define EF 8035   // 311.127 Hz	
//#define E 7584   // 329.628 Hz	
//#define F 7159   // 349.228 Hz	
//#define GF 6757   // 369.994 Hz	
//#define G 6378   // 391.995 Hz	
//#define AF 6020   // 415.305 Hz	
//#define A 5682   // 440.000 Hz	
//#define BF 5363   // 466.164 Hz	
//#define B 5062   // 493.883 Hz	
//#define C 4778   // 523.251 Hz	
//#define DF1 4510   // 554.365 Hz	

#define QUIET					0				// no sound
#define REPEAT				1				// repeat from beginning
#define STOP					2				// stop and restart
#define EIGHT_NOTE		45				// sets tempo, units 4.096ms
#define QUARTER_NOTE	2*EIGHT_NOTE	// 163 beats/min
#define HALF_NOTE			4*EIGHT_NOTE
#define PAUSE					3				// units 4.096ms, 0.012s

//uint16_t Wave[32] = {  
//  1024,1122,1215,1302,1378,1440,1486,1514,1524,1514,1486,
//  1440,1378,1302,1215,1122,1024,926,833,746,670,608,
//  562,534,524,534,562,608,670,746,833,926
//};  

const unsigned short Trumpet[32] = {  	
  1007,1088,1156,1194,1067,789,303,99,789,1510,1476,	
  1173,1067,1037,1084,1062,1011,1015,1045,1062,1011,1011,	
  1058,1113,1084,1075,1079,1105,1088,1049,1015,1045	
};

uint16_t odeToJoy[32] = {2048,2048,2047,2048,2048,2047,2047,2048,2048,2048,2047,2048,2047,2048,2048,2048,2047,2048,2047,2048,2048,2047,2047,2047,2048,2048,2048,2048,2047,2048,2047,2048};
uint16_t ode_beg[64] = {2048,2048,2047,
2048,2047,2047,
2047,2048,2048,
2048,2048,2047,
2048,2048,2048,
2048,2047,2048,
2048,2047,2047,
2047,2048,2048,
2048,2048,2047,
2048,2047,2048,
2048,2047,2048,
2047,2048,2047,
2047,2048,2047,
2048,2048,2047,
2048,2048,2048,
2048,2048,2048,
2047,2047,2048,
2048,2047,2047,
2048,2047,2047,
2047,2048,2047,
2048,2047,2048,2048};
struct MusicState{
	uint32_t Pitch0;		// freq = 375000/Pitch
	uint16_t Duration;	// number of 4.096ms intervals
};

typedef const struct MusicState MusicStateType;
int32_t Position = 0;			// index into SongTable
int32_t Play = 0;					// true if song is playing
uint32_t  SongCount = 0;	// counter for play current note

MusicStateType SongTable[]={	// Ode to Joy
//1
	{E, QUARTER_NOTE},
	{E, QUARTER_NOTE},
	{F, QUARTER_NOTE},
	{G, QUARTER_NOTE},
	{QUIET, PAUSE},
//2
	{G, QUARTER_NOTE},
	{F, QUARTER_NOTE},
	{E, QUARTER_NOTE},
	{D, QUARTER_NOTE},
	{QUIET, PAUSE},
//3
	{C0, QUARTER_NOTE},
	{C0, QUARTER_NOTE},
	{D, QUARTER_NOTE},
	{E, QUARTER_NOTE},
	{QUIET, PAUSE},
//4
	{E, EIGHT_NOTE*3},
	{D, EIGHT_NOTE},
	{D, HALF_NOTE},
	{QUIET, PAUSE},
//5
	{E, QUARTER_NOTE},
	{E, QUARTER_NOTE},
	{F, QUARTER_NOTE},
	{G, QUARTER_NOTE},
	{QUIET, PAUSE},
//6
	{G, QUARTER_NOTE},
	{F, QUARTER_NOTE},
	{E, QUARTER_NOTE},
	{D, QUARTER_NOTE},
	{QUIET, PAUSE},
//7
	{C0, QUARTER_NOTE},
	{C0, QUARTER_NOTE},
	{D, QUARTER_NOTE},
	{E, QUARTER_NOTE},
	{QUIET, PAUSE},
//8
	{D, EIGHT_NOTE*3},
	{C0, EIGHT_NOTE},
	{C0, HALF_NOTE},
	{QUIET, PAUSE},
//9
	{D, HALF_NOTE},
	{E, QUARTER_NOTE},
	{C0, QUARTER_NOTE},
	{QUIET, PAUSE},
//10
	{D, QUARTER_NOTE},
	{E, EIGHT_NOTE},
	{F, EIGHT_NOTE},
	{E, QUARTER_NOTE},
	{C0, QUARTER_NOTE},
	{QUIET, PAUSE},
//11
	{D, QUARTER_NOTE},
	{E, EIGHT_NOTE},
	{F, EIGHT_NOTE},
	{E, QUARTER_NOTE},
	{D, QUARTER_NOTE},
	{QUIET, PAUSE},
//12
	{C0, QUARTER_NOTE},
	{D, QUARTER_NOTE},
	{G, HALF_NOTE},
	{QUIET, PAUSE},
//13
	{E, QUARTER_NOTE},
	{E, QUARTER_NOTE},
	{F, QUARTER_NOTE},
	{G, QUARTER_NOTE},
	{QUIET, PAUSE},
//14
	{G, QUARTER_NOTE},
	{F, QUARTER_NOTE},
	{E, QUARTER_NOTE},
	{D, QUARTER_NOTE},
	{QUIET, PAUSE},
//15
	{C0, QUARTER_NOTE},
	{C0, QUARTER_NOTE},
	{D, QUARTER_NOTE},
	{E, QUARTER_NOTE},
	{QUIET, PAUSE},
//16
	{D, EIGHT_NOTE*3},
	{C0, EIGHT_NOTE},
	{C0, HALF_NOTE},
	{QUIET, PAUSE} 
};

uint32_t I0 = 0; 
const uint16_t *TheWave;
uint32_t Period0 = 0; 		//Index to the Pitch FREQ
uint32_t Out0 = 0;

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
void Timer_Arm(void){
	TIMER1_CTL_R |= 0x01;
	TIMER1_TAILR_R = Period0-1;    // 4) reload value
}

//------------Timer_Disarm-----------------
void Timer_Disarm(void){
	TIMER1_CTL_R &= ~0x01;
}

//void Rewind(void){
//	Play = 0;
//	Position = 0;
//	I0 = 0;
//	Timer_Disarm();
//}
void Play_Sound(void){
		DAC_Out(ode_beg[I0]<<1);
	//DAC_Out(A);
		//DAC_Out(C);
	//DAC_Out(E);
		I0 = (I0+1)%64;
}

void NextNote(void){
	//Timer_Disarm();
	//Timer1A_Stop();
	//Period0 = SongTable[Position].Pitch0;
//	if(Period0 == REPEAT){
//		Position = 0;
//		Period0 = SongTable[Position].Pitch0;
//	}
	SongCount = SongTable[Position].Duration;
	Period0 = SongTable[Position].Pitch0;
	Timer1A_Init(&Play_Sound, C, 1);
	//Timer1A_Init(&Play_Sound, Period0, 1);
	//Timer1A_Init(&Play_Sound, A, 1);
//	if(Period0 == STOP){
//		Rewind();
//	}
	//Timer_Arm();
}

void SongHandler(void){
	if(SongCount ==0){
		//I0 = 0;
		Position = (Position+1)%77;
		NextNote();
	}
	else{
		SongCount--;
	}
}
//void SongHandler(void){
//	int static lastPlay=0;
//	int static lastVoice=0;
//	int thisPlay, thisVoice;
//}
	//Mailbox_Send((Period0<<16)+Period0);
