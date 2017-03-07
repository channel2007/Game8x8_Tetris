/*****************************************************************************
*  8x8時脈系統.
*
*  created  by channel
*
*  todo list:
*
*  note:
****************************************************************************/

#ifndef _GAME8X8TIME_h
#define _GAME8X8TIME_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Arduino.h>

class Game8x8Time
{
 private:
	 // 相差時間.
	 unsigned long timeTemp = 0;
	 // 相差時間累積.
	 unsigned long timeTick = 0;

 public:
	 // 建構式.
	 Game8x8Time();

	 // 更新.
	 bool update(unsigned long tick, bool cls = true);
};


#endif

