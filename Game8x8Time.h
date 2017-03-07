/*****************************************************************************
*  8x8�ɯߨt��.
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
	 // �ۮt�ɶ�.
	 unsigned long timeTemp = 0;
	 // �ۮt�ɶ��ֿn.
	 unsigned long timeTick = 0;

 public:
	 // �غc��.
	 Game8x8Time();

	 // ��s.
	 bool update(unsigned long tick, bool cls = true);
};


#endif

