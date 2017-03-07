/*****************************************************************************
* Game8x8-�Xù������C��.
*
* created  by channel
*
* todo list:
*  -[]�쩳��n��1���ɶ��i�H����
*
* note:
*
****************************************************************************/
#include "Game8x8Tone.h"
#include "Game8x8JoyStick.h"
#include "Game8x8Time.h"
#include "Game8x8Graphics.h"

// ø�Ϩ��.
Game8x8Graphics game8x8Graphics = Game8x8Graphics();
// �n����.
Game8x8JoyStick game8x8JoyStick = Game8x8JoyStick();
// ���ﾹ���.
Game8x8Tone		game8x8Tone = Game8x8Tone();

// ø�ϧ�s�ɯ�.
Game8x8Time TimeGraphicsUpdate = Game8x8Time();
// ��������ɯ�.
Game8x8Time TimeMoveDown = Game8x8Time();
// ���k���ʤ���ɯ�.
Game8x8Time TimeMoveRL = Game8x8Time();
// �{�{���.
Game8x8Time TimeBrickFlash = Game8x8Time();


// �]�w����Ҧ����A.
byte N1[2][4] = { { 0,3,4,7 },{ 4,5,6,7 } };							// ID:1.
byte N2[2][4] = { { 1,3,4,6 },{ 3,4,7,8 } };							// ID:2.
byte L1[4][4] = { { 3,6,7,8 },{ 0,1,3,6 },{ 3,4,5,8 },{ 1,4,6,7 } };	// ID:3.
byte L2[4][4] = { { 5,6,7,8 },{ 0,3,6,7 },{ 3,4,5,6 },{ 0,1,4,7 } };	// ID:4.
byte T[4][4]  = { { 4,6,7,8 },{ 0,3,4,6 },{ 3,4,5,7 },{ 1,3,4,7 } };	// ID:5.
byte O[1][4]  = { { 3,4,6,7 } };										// ID:6.
byte I[2][3]  = { { 6,7,8 },{ 1,4,7 } };			 					// ID:7.

// ����}�C.
byte brick[3][3] = {
	{ 0,0,0 },
	{ 0,0,0 },
	{ 0,0,0 },
};

// �e��.
//  0:�L.
// >0:����s��.
//255:�{�{���(��).
//254:�{�{���(�G).
byte container[8][8] = {
	{ 0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0 },
};

// ����b�e������m.
int containerX =  3;		// ( 0~6)(  ��6���ɭԤ��������).
int containerY = -2;		// (-2~5)(-2��ܦb�W��ɥ~�C�C���U��).

// ����s��(1~7).
byte brickId = 1;
// ������A(0~3).
int  brickState = 0;

// �M������ƶq.
byte clearBrickFlag = 0;
// �M������{�{����.
byte flashNum = 0;

// ����U���t��.
int downSpeed = 1000;
int downSpeedTemp = 0;

// �C�����A.
// 0:�ǳƶ}�l��.
// 1:�C����.
byte gameStatus = 0;

// �B�z�ܴ������V�X��.
bool JoyUp = false;
bool JoyDown = false;

// ���Ķ}��.
bool soundSwitch = true;

//---------------------------------------------------------------------------
// ��l.
//---------------------------------------------------------------------------
void setup()
{
	Serial.begin(9600);

	// �����H����seed.
	randomSeed(analogRead(0));
	// ���s�}�l�C��.
	resetGame();
}

//---------------------------------------------------------------------------
// �D�j��.
//---------------------------------------------------------------------------
void loop()
{
	int posX = 0;
	int posY = 0;

	// 0:�ǳƶ}�l��.
	if (gameStatus == 0) {
		// �n�쩹�U-�}�l�C��.
		if (game8x8JoyStick.getJoyY() < 3) {
			// �M������ƶq.
			clearBrickFlag = 0;
			// �]�w�C����.
			gameStatus = 1;
		}

	// 1:�C����.
	}else if (gameStatus == 1) {
		// ��������ɯ�.
		if (TimeMoveDown.update(downSpeed) && clearBrickFlag == 0) {
			// ���񭵮�.
			if (soundSwitch) {
				// �o�X�n��.
				game8x8Tone.playTone(262, 33);
			}
			// ������U��.
			containerY++;
			// �P�_�O�_�I���L���.
			if (!ifCopyToContainer()) {
				// ���W���@��.
				containerY--;
				// �C������.
				if (containerY < 0) {
					// ���s�}�l�C��.
					resetGame();
					return;
				}
				// ���񭵮�.
				if (soundSwitch) {
					// �o�X�n��.
					game8x8Tone.playTone(494, 33);
				}
				// ���ͷs��.
				brickNew();
			}
			// �w�쩳.
			else if (containerY >= 5) {
				// ���񭵮�.
				if (soundSwitch) {
					// �o�X�n��.
					game8x8Tone.playTone(494, 33);
				}
				// ���ͷs��.
				brickNew();
			}
			// �P�_�P�]�w�n�M�������.
			clearBrickFlag = ifClearBrick();
		}

		//--------------------------------------------------------------------		
		// �B�z�{�{�M�M�����.
		if (clearBrickFlag > 0)
		{
			// �{�{���.
			if (TimeBrickFlash.update(100)) {
				// ���񭵮�.
				if (soundSwitch) {
					// �o�X�n��.
					game8x8Tone.playTone(440, 33);
				}
				// �{�{����.
				flashNum++;
				// �{�{���.
				brickFlash();
				// �M������~��C��.
				if (flashNum >= 3) {
					// �M�������.
					clearBrick();
					// ��l�ܼ�.
					clearBrickFlag = 0;
					flashNum = 0;
				}
			}
		}
	}
	
	//--------------------------------------------------------------------
	// ø�ϧ�s�ɯ�-FPS 30.
	if (TimeGraphicsUpdate.update(33)) {
	
		// �M���e��.
		game8x8Graphics.clearDisplay(0);

		//--------------------------------------------------------------------
		// �n�쩹�W�ާ@�ܴ����.
		if (game8x8JoyStick.getJoyY() > 9 && !JoyUp && containerX != -1 && containerX != 6 && clearBrickFlag == 0) {
			// ���񭵮�.
			if (soundSwitch) {
				// �o�X�n��.
				game8x8Tone.playTone(294, 33);
			}
			// N1�BN2�BI(2�ت��A).
			if ( brickId == 1 || brickId == 2 || brickId == 7) {
				brickState++;
				if (brickState > 1) { brickState = 0; }

				// �B�z�P�_�O�_�i�H������.
				processTransformToBrickArray();
				if (!ifCopyToContainer()) {
					brickState--;
					if (brickState < 0) { brickState = 1; }
				}
			}
			// L1�BL2�BT(4�ت��A)
			else if (brickId == 3 || brickId == 4 || brickId == 5) {
				brickState++;
				if (brickState > 3) { brickState = 0; }

				// �B�z�P�_�O�_�i�H������.
				processTransformToBrickArray();
				if (!ifCopyToContainer()) {
					brickState--;
					if (brickState < 0) { brickState = 3; }
				}
			}
			// O(1�ت��A)
			else if (brickId == 6) {
				brickState = 0;
			}
			JoyUp = true;
		}
		else if (game8x8JoyStick.getJoyY() <= 9) {
			JoyUp = false;
		}

		// �n�쩹�U-�ֳt�U�����.
		if (game8x8JoyStick.getJoyY() < 3 && clearBrickFlag == 0) {
			// �����쥻�U���t��.
			if (downSpeedTemp == 0) {
				downSpeedTemp = downSpeed;
			}
			// ���ܳt��.
			downSpeed = 100;
		}
		else if (game8x8JoyStick.getJoyY() >= 3 && clearBrickFlag == 0) {			
			// ��_�쥻�t��.
			if (downSpeedTemp != 0) {
				downSpeed = downSpeedTemp;
				downSpeedTemp = 0;
			}
		}

		/*
		// �n�쩹�U-�ܴ����.
		if (game8x8JoyStick.getJoyY() < 3 && !JoyDown) {
			brickId++;
			if (brickId > 7) { brickId = 1; }
			brickState = 0;
			JoyDown = true;
		}else if (game8x8JoyStick.getJoyY() >= 3) {			
			JoyDown = false;
		}
		*/

		//--------------------------------------------------------------------
		// �B�z�ഫ��������}�C.
		processTransformToBrickArray();

		//--------------------------------------------------------------------
		// �ާ@���k���ʤ��.
		if (TimeMoveRL.update(100) && clearBrickFlag == 0) {
			int mx = game8x8JoyStick.getJoyX();
			if (mx != 5) {
				// �k��.
				if (mx < 5) {
					containerX++;
					// �P�_�i�_�k��.
					if (ifCopyToContainer()) {
						if (brick[2][0] == 0 && brick[2][1] == 0 && brick[2][2] == 0) {
							if (containerX>6) { containerX = 6; }
						}
						else {
							if (containerX>5) { containerX = 5; }
						}
					}
					else {
						containerX--;
					}
				}
				// ����.
				else
				{
					containerX--;
					// �P�_�i�_����.
					if (ifCopyToContainer()) {
						if (containerX < 0) { 
							// �B�z���ߪ���(�S��).
							if (brick[0][0] == 0 && brick[0][1] == 0 && brick[0][2] == 0) {
								containerX = -1;
							}
							else {
								containerX = 0;
							}
						}
					}
					else {
						containerX++;
					}					
				}
			}
		}

		//--------------------------------------------------------------------		
		// �e�e��.
		for (int iy = 0; iy<8; iy++) {
			for (int ix = 0; ix<8; ix++) {
				if (container[ix][iy] > 0 && container[ix][iy] != 255) {
					game8x8Graphics.setPixcls(ix, iy, 1);
				}
				else {
					game8x8Graphics.setPixcls(ix, iy, 0);
				}				
			}
		}

		// �e���.		
		for (int iy = 0; iy < 3; iy++) {
			for (int ix = 0; ix < 3; ix++) {
				posX = containerX + ix;
				posY = containerY + iy;
				if( posX >= 0 && posY >= 0){
					if (brick[ix][iy] > 0) {
						game8x8Graphics.setPixcls( posX, posY, 1);
					}
					else {
						// �B�z�W�X��ɤ��n�e.
						if (posX <= 5) {
							// �e����O�Ū��~�e.
							if(container[posX][posY] == 0){
								game8x8Graphics.setPixcls(posX, posY, 0);
							}							
						}						
					}
				}
			}
		}
		// ��s.
		game8x8Graphics.update();
	}

}

//--------------------------------------------------------------------
// �B�z�ഫ��������}�C.
//--------------------------------------------------------------------
void processTransformToBrickArray()
{
	// N1.
	if (brickId == 1) {
		transformToBrickArray((byte *)N1, 4, brickState, brickId);
	}
	// N2.
	else if (brickId == 2) {
		transformToBrickArray((byte *)N2, 4, brickState, brickId);
	}
	// L1.
	else if (brickId == 3) {
		transformToBrickArray((byte *)L1, 4, brickState, brickId);
	}
	// L2.
	else if (brickId == 4) {
		transformToBrickArray((byte *)L2, 4, brickState, brickId);
	}
	// T.
	else if (brickId == 5) {
		transformToBrickArray((byte *)T, 4, brickState, brickId);
	}
	// O.
	else if (brickId == 6) {
		transformToBrickArray((byte *)O, 4, brickState, brickId);
	}
	// I.
	else if (brickId == 7) {
		transformToBrickArray((byte *)I, 3, brickState, brickId);
	}
}

//---------------------------------------------------------------------------
// �ഫ��������}�C.
//	pBrick	: ����}�C.
//	bLengthY: ����j�p.
//	bState	: ������A.
//  bBrickId: ����s��.
//---------------------------------------------------------------------------
void transformToBrickArray(byte *pBrick, byte bLengthY, byte bState, byte bBrickId)
{
	byte bx, by;

	//(0,1).
	//Serial.println (*(pBrick + 0 * lengthY + 1));

	// �M������}�C.
	for (int iy = 0; iy < 3; iy++) {
		for (int ix = 0; ix < 3; ix++) {
			brick[ix][iy] = 0;
		}		
	}

	// �ഫ��������}�C.
	for (int i = 0; i < bLengthY; i++) {
		bx = (*(pBrick + bState * bLengthY + i)) % 3;
		by = (*(pBrick + bState * bLengthY + i)) / 3;
		brick[bx][by] = bBrickId;
	}

	/*
	// �L�X�T��.
	for (int iy = 0; iy < 3; iy++) {
		for (int ix = 0; ix < 3; ix++) {
			Serial.print(brick[ix][iy], DEC);
		}
		Serial.print("\n");
	}
	*/
}

//---------------------------------------------------------------------------
// �P�_�O�_�i�H�ƻs��e����.
//	true:�i�H.  false:���i�H.
//---------------------------------------------------------------------------
bool ifCopyToContainer() {
	int posX = 0;
	int posY = 0;
	for (int iy = 0; iy < 3; iy++) {
		for (int ix = 0; ix < 3; ix++) {
			if (brick[ix][iy] != 0) {
				posX = containerX + ix;
				posY = containerY + iy;
				if (posX >= 0 && posY >= 0) {
					if (container[posX][posY] != 0) {
						return false;
					}
				}
			}
		}
	}
	return true;
}

//---------------------------------------------------------------------------
// �ƻs�����e����.
//---------------------------------------------------------------------------
void copyToContainer() {
	int posX = 0;
	int posY = 0;
	for (int iy = 0; iy < 3; iy++) {
		for (int ix = 0; ix < 3; ix++) {
			if (brick[ix][iy] != 0) {
				posX = containerX + ix;
				posY = containerY + iy;
				if (posX >= 0 && posY >= 0) {
					container[posX][posY] = brick[ix][iy];
				}
				
			}
		}
	}
}

//---------------------------------------------------------------------------
// �P�_�P�]�w�n�M�������.
//---------------------------------------------------------------------------
byte ifClearBrick() {
	int pointNum = 0;	
	int lineNum = 0;
	for (int iy = 0; iy < 8; iy++) {
		for (int ix = 0; ix < 8; ix++) {
			if (container[ix][iy] > 0) {
				pointNum++;
			}
			if (pointNum == 8) {
				for (int i = 0; i < 8; i++) {
					lineNum++;
					container[i][iy] = 255;
				}
			}
		}
		pointNum = 0;
	}
	return lineNum;
}

//---------------------------------------------------------------------------
// �M�������.
//---------------------------------------------------------------------------
void clearBrick() {
	byte temp = 0;

	// �@�C�@�C�P�_�M�����.
	for (int ix = 0; ix < 8; ix++) {
		for (int iu = 0; iu < 7; iu++) {
			for (int iy = 0; iy < 8; iy++) {
				if (container[ix][iy] == 255 || container[ix][iy] == 254) {
					if (iy > 0) {
						temp = container[ix][iy - 1];
						container[ix][iy - 1] = container[ix][iy];
						container[ix][iy] = temp;
						iy--;
					}
				}
			}
			container[ix][0] = 0;
		}
	}

	/*
	// �L�X�T��.
	for (int iy = 0; iy < 8; iy++) {
		for (int ix = 0; ix < 8; ix++) {
			Serial.print(container[ix][iy], DEC);
		}
		Serial.print("\n");
	}
	*/
}

//---------------------------------------------------------------------------
// �{�{���.
//---------------------------------------------------------------------------
void brickFlash() {
	for (int iy = 0; iy < 8; iy++) {
		for (int ix = 0; ix < 8; ix++) {
			if (container[ix][iy] == 255) {
				container[ix][iy] = 254;
			}
			else if (container[ix][iy] == 254) {
				container[ix][iy] = 255;
			}
		}
	}

	/*
	// �L�X�T��.
	for (int iy = 0; iy < 8; iy++) {
		for (int ix = 0; ix < 8; ix++) {
			Serial.print(container[ix][iy], DEC);
		}
		Serial.print("\n");
	}
	*/
}

//---------------------------------------------------------------------------
// ���ͷs��.
//---------------------------------------------------------------------------
void brickNew(){
	// �ƻs�����e����.
	copyToContainer();
	// ��l���.
	containerX = 3;
	containerY = -2;
	// �üƲ��ͤ��.
	brickId = (byte)random(1, 8);
	// ��l������A.
	brickState = 0;
}

//---------------------------------------------------------------------------
// ���s�}�l�C��.
//---------------------------------------------------------------------------
void resetGame()
{
	// �C�����A.
	gameStatus = 0;

	// �U���t��.
	downSpeed = 1000;
	downSpeedTemp = 0;

	// �üƲ��ͤ��.
	brickId = (byte)random(1, 8);
	// ������A(0~3).
	brickState = 0;

	// ����b�e������m.
	containerX = 3;
	if (brickId == 1 || brickId == 2) {
		containerY = 0;
	}
	else if (brickId == 7) {
		containerY = -2;
	}
	else {
		containerY = -1;
	}
	
	// �M������ƶq(�Ȱ�).
	clearBrickFlag = 1;

	// �M���e��.
	for (int iy = 0; iy < 8; iy++) {
		for (int ix = 0; ix < 8; ix++) {
			container[ix][iy] = 0;
		}
	}	
}