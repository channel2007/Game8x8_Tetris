/*****************************************************************************
*  8x8蜂鳴器.
*
*  created  by channel
*
*  todo list:
*
*  note:
****************************************************************************/

#ifndef _GAME8X8TONE_h
#define _GAME8X8TONE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Arduino.h>

#define Do		262
#define Re		294
#define Mi		330
#define Fa		349
#define Sol		392
#define La		440
#define Si		494

#define Do_h	523
#define Re_h	587
#define Mi_h	659
#define Fa_h	698
#define Sol_h	784
#define La_h	880
#define Si_h	988

class Game8x8Tone
{
private:
	// pin編號.
	const static byte SP_PIN = 9;

public:
	// 建構式.
	Game8x8Tone();

	// 播放音效.
	void playTone( unsigned int frequency, unsigned long duration = 0);

};

#endif

