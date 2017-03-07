/*****************************************************************************
*  8x8�n��w.
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

	// ���o�ƭ�.
	int value = 0;

public:
	// �غc��.
	Game8x8JoyStick();

	// x.
	int getJoyX();

	// y.
	int getJoyY();

	// z.
	int getJoyZ();

};

#endif

