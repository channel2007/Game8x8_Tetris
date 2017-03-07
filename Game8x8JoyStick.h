/*****************************************************************************
*  8x8搖桿庫.
*
*  created  by channel
*
*  todo list:
*
*  note:
****************************************************************************/

#ifndef _GAME8X8JOYSTICK_h
#define _GAME8X8JOYSTICK_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Arduino.h>

class Game8x8JoyStick
{
private:

	// 取得數值.
	int value = 0;

public:
	// 建構式.
	Game8x8JoyStick();

	// x.
	int getJoyX();

	// y.
	int getJoyY();

	// z.
	int getJoyZ();

};

#endif

