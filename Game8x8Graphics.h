/*****************************************************************************
*  8x8繪圖庫.
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
	// 畫布.
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

	// 控制MAX7219函數庫.
	LedControl *lc;

	// 建構式.
	Game8x8Graphics();

	// 更新.
	void update();

	// 清除.
	void clearDisplay(byte state);
	// 設定像素.
	void setPixcls(int row, int col, byte state);

};

#endif

