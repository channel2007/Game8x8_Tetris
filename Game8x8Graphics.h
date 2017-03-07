/*****************************************************************************
*  8x8ø�Ϯw.
*
*  created  by channel
*
*  todo list:
*
*  note:
****************************************************************************/

#ifndef _GAME8X8GRAPHICS_h
#define _GAME8X8GRAPHICS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Arduino.h>
#include "LedControl.h"

class Game8x8Graphics {
private:
	// �e��.
	byte canvas[8][8] = {
		{ 0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0 },
	};

public:

	// ����MAX7219��Ʈw.
	LedControl *lc;

	// �غc��.
	Game8x8Graphics();

	// ��s.
	void update();

	// �M��.
	void clearDisplay(byte state);
	// �]�w����.
	void setPixcls(int row, int col, byte state);

};

#endif

