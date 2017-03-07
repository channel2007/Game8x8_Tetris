/*****************************************************************************
* Game8x8-俄羅斯方塊遊戲.
*
* created  by channel
*
* todo list:
*  -[]到底後要有1秒的時間可以移動
*
* note:
*
****************************************************************************/
#include "Game8x8Tone.h"
#include "Game8x8JoyStick.h"
#include "Game8x8Time.h"
#include "Game8x8Graphics.h"

// 繪圖函數.
Game8x8Graphics game8x8Graphics = Game8x8Graphics();
// 搖桿函數.
Game8x8JoyStick game8x8JoyStick = Game8x8JoyStick();
// 蜂鳴器函數.
Game8x8Tone		game8x8Tone = Game8x8Tone();

// 繪圖更新時脈.
Game8x8Time TimeGraphicsUpdate = Game8x8Time();
// 掉落方塊時脈.
Game8x8Time TimeMoveDown = Game8x8Time();
// 左右移動方塊時脈.
Game8x8Time TimeMoveRL = Game8x8Time();
// 閃爍方塊.
Game8x8Time TimeBrickFlash = Game8x8Time();


// 設定方塊所有狀態.
byte N1[2][4] = { { 0,3,4,7 },{ 4,5,6,7 } };							// ID:1.
byte N2[2][4] = { { 1,3,4,6 },{ 3,4,7,8 } };							// ID:2.
byte L1[4][4] = { { 3,6,7,8 },{ 0,1,3,6 },{ 3,4,5,8 },{ 1,4,6,7 } };	// ID:3.
byte L2[4][4] = { { 5,6,7,8 },{ 0,3,6,7 },{ 3,4,5,6 },{ 0,1,4,7 } };	// ID:4.
byte T[4][4]  = { { 4,6,7,8 },{ 0,3,4,6 },{ 3,4,5,7 },{ 1,3,4,7 } };	// ID:5.
byte O[1][4]  = { { 3,4,6,7 } };										// ID:6.
byte I[2][3]  = { { 6,7,8 },{ 1,4,7 } };			 					// ID:7.

// 方塊陣列.
byte brick[3][3] = {
	{ 0,0,0 },
	{ 0,0,0 },
	{ 0,0,0 },
};

// 容器.
//  0:無.
// >0:方塊編號.
//255:閃爍方塊(黑).
//254:閃爍方塊(亮).
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

// 方塊在容器的位置.
int containerX =  3;		// ( 0~6)(  為6的時候不能旋轉方塊).
int containerY = -2;		// (-2~5)(-2表示在上邊界外慢慢往下掉).

// 方塊編號(1~7).
byte brickId = 1;
// 方塊狀態(0~3).
int  brickState = 0;

// 清除方塊數量.
byte clearBrickFlag = 0;
// 清除方塊閃爍次數.
byte flashNum = 0;

// 方塊下降速度.
int downSpeed = 1000;
int downSpeedTemp = 0;

// 遊戲狀態.
// 0:準備開始中.
// 1:遊戲中.
byte gameStatus = 0;

// 處理變換方塊方向旗標.
bool JoyUp = false;
bool JoyDown = false;

// 音效開關.
bool soundSwitch = true;

//---------------------------------------------------------------------------
// 初始.
//---------------------------------------------------------------------------
void setup()
{
	Serial.begin(9600);

	// 產生隨機的seed.
	randomSeed(analogRead(0));
	// 重新開始遊戲.
	resetGame();
}

//---------------------------------------------------------------------------
// 主迴圈.
//---------------------------------------------------------------------------
void loop()
{
	int posX = 0;
	int posY = 0;

	// 0:準備開始中.
	if (gameStatus == 0) {
		// 搖桿往下-開始遊戲.
		if (game8x8JoyStick.getJoyY() < 3) {
			// 清除方塊數量.
			clearBrickFlag = 0;
			// 設定遊戲中.
			gameStatus = 1;
		}

	// 1:遊戲中.
	}else if (gameStatus == 1) {
		// 方塊掉落時脈.
		if (TimeMoveDown.update(downSpeed) && clearBrickFlag == 0) {
			// 播放音效.
			if (soundSwitch) {
				// 發出聲音.
				game8x8Tone.playTone(262, 33);
			}
			// 方塊往下掉.
			containerY++;
			// 判斷是否碰到其他方塊.
			if (!ifCopyToContainer()) {
				// 往上移一格.
				containerY--;
				// 遊戲結束.
				if (containerY < 0) {
					// 重新開始遊戲.
					resetGame();
					return;
				}
				// 播放音效.
				if (soundSwitch) {
					// 發出聲音.
					game8x8Tone.playTone(494, 33);
				}
				// 產生新塊.
				brickNew();
			}
			// 已到底.
			else if (containerY >= 5) {
				// 播放音效.
				if (soundSwitch) {
					// 發出聲音.
					game8x8Tone.playTone(494, 33);
				}
				// 產生新塊.
				brickNew();
			}
			// 判斷與設定要清除的方塊.
			clearBrickFlag = ifClearBrick();
		}

		//--------------------------------------------------------------------		
		// 處理閃爍和清除方塊.
		if (clearBrickFlag > 0)
		{
			// 閃爍方塊.
			if (TimeBrickFlash.update(100)) {
				// 播放音效.
				if (soundSwitch) {
					// 發出聲音.
					game8x8Tone.playTone(440, 33);
				}
				// 閃爍次數.
				flashNum++;
				// 閃爍方塊.
				brickFlash();
				// 清除方塊繼續遊戲.
				if (flashNum >= 3) {
					// 清除的方塊.
					clearBrick();
					// 初始變數.
					clearBrickFlag = 0;
					flashNum = 0;
				}
			}
		}
	}
	
	//--------------------------------------------------------------------
	// 繪圖更新時脈-FPS 30.
	if (TimeGraphicsUpdate.update(33)) {
	
		// 清除畫面.
		game8x8Graphics.clearDisplay(0);

		//--------------------------------------------------------------------
		// 搖桿往上操作變換方塊.
		if (game8x8JoyStick.getJoyY() > 9 && !JoyUp && containerX != -1 && containerX != 6 && clearBrickFlag == 0) {
			// 播放音效.
			if (soundSwitch) {
				// 發出聲音.
				game8x8Tone.playTone(294, 33);
			}
			// N1、N2、I(2種狀態).
			if ( brickId == 1 || brickId == 2 || brickId == 7) {
				brickState++;
				if (brickState > 1) { brickState = 0; }

				// 處理判斷是否可以旋轉方塊.
				processTransformToBrickArray();
				if (!ifCopyToContainer()) {
					brickState--;
					if (brickState < 0) { brickState = 1; }
				}
			}
			// L1、L2、T(4種狀態)
			else if (brickId == 3 || brickId == 4 || brickId == 5) {
				brickState++;
				if (brickState > 3) { brickState = 0; }

				// 處理判斷是否可以旋轉方塊.
				processTransformToBrickArray();
				if (!ifCopyToContainer()) {
					brickState--;
					if (brickState < 0) { brickState = 3; }
				}
			}
			// O(1種狀態)
			else if (brickId == 6) {
				brickState = 0;
			}
			JoyUp = true;
		}
		else if (game8x8JoyStick.getJoyY() <= 9) {
			JoyUp = false;
		}

		// 搖桿往下-快速下降方塊.
		if (game8x8JoyStick.getJoyY() < 3 && clearBrickFlag == 0) {
			// 紀錄原本下降速度.
			if (downSpeedTemp == 0) {
				downSpeedTemp = downSpeed;
			}
			// 改變速度.
			downSpeed = 100;
		}
		else if (game8x8JoyStick.getJoyY() >= 3 && clearBrickFlag == 0) {			
			// 恢復原本速度.
			if (downSpeedTemp != 0) {
				downSpeed = downSpeedTemp;
				downSpeedTemp = 0;
			}
		}

		/*
		// 搖桿往下-變換方塊.
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
		// 處理轉換方塊到方塊陣列.
		processTransformToBrickArray();

		//--------------------------------------------------------------------
		// 操作左右移動方塊.
		if (TimeMoveRL.update(100) && clearBrickFlag == 0) {
			int mx = game8x8JoyStick.getJoyX();
			if (mx != 5) {
				// 右移.
				if (mx < 5) {
					containerX++;
					// 判斷可否右移.
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
				// 左移.
				else
				{
					containerX--;
					// 判斷可否左移.
					if (ifCopyToContainer()) {
						if (containerX < 0) { 
							// 處理直立長條(特例).
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
		// 畫容器.
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

		// 畫方塊.		
		for (int iy = 0; iy < 3; iy++) {
			for (int ix = 0; ix < 3; ix++) {
				posX = containerX + ix;
				posY = containerY + iy;
				if( posX >= 0 && posY >= 0){
					if (brick[ix][iy] > 0) {
						game8x8Graphics.setPixcls( posX, posY, 1);
					}
					else {
						// 處理超出邊界不要畫.
						if (posX <= 5) {
							// 容器位是空的才畫.
							if(container[posX][posY] == 0){
								game8x8Graphics.setPixcls(posX, posY, 0);
							}							
						}						
					}
				}
			}
		}
		// 更新.
		game8x8Graphics.update();
	}

}

//--------------------------------------------------------------------
// 處理轉換方塊到方塊陣列.
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
// 轉換方塊到方塊陣列.
//	pBrick	: 方塊陣列.
//	bLengthY: 方塊大小.
//	bState	: 方塊狀態.
//  bBrickId: 方塊編號.
//---------------------------------------------------------------------------
void transformToBrickArray(byte *pBrick, byte bLengthY, byte bState, byte bBrickId)
{
	byte bx, by;

	//(0,1).
	//Serial.println (*(pBrick + 0 * lengthY + 1));

	// 清除方塊陣列.
	for (int iy = 0; iy < 3; iy++) {
		for (int ix = 0; ix < 3; ix++) {
			brick[ix][iy] = 0;
		}		
	}

	// 轉換方塊到方塊陣列.
	for (int i = 0; i < bLengthY; i++) {
		bx = (*(pBrick + bState * bLengthY + i)) % 3;
		by = (*(pBrick + bState * bLengthY + i)) / 3;
		brick[bx][by] = bBrickId;
	}

	/*
	// 印出訊息.
	for (int iy = 0; iy < 3; iy++) {
		for (int ix = 0; ix < 3; ix++) {
			Serial.print(brick[ix][iy], DEC);
		}
		Serial.print("\n");
	}
	*/
}

//---------------------------------------------------------------------------
// 判斷是否可以複製到容器內.
//	true:可以.  false:不可以.
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
// 複製方塊到容器內.
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
// 判斷與設定要清除的方塊.
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
// 清除的方塊.
//---------------------------------------------------------------------------
void clearBrick() {
	byte temp = 0;

	// 一列一列判斷清除方塊.
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
	// 印出訊息.
	for (int iy = 0; iy < 8; iy++) {
		for (int ix = 0; ix < 8; ix++) {
			Serial.print(container[ix][iy], DEC);
		}
		Serial.print("\n");
	}
	*/
}

//---------------------------------------------------------------------------
// 閃爍方塊.
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
	// 印出訊息.
	for (int iy = 0; iy < 8; iy++) {
		for (int ix = 0; ix < 8; ix++) {
			Serial.print(container[ix][iy], DEC);
		}
		Serial.print("\n");
	}
	*/
}

//---------------------------------------------------------------------------
// 產生新塊.
//---------------------------------------------------------------------------
void brickNew(){
	// 複製方塊到容器內.
	copyToContainer();
	// 初始方塊.
	containerX = 3;
	containerY = -2;
	// 亂數產生方塊.
	brickId = (byte)random(1, 8);
	// 初始方塊狀態.
	brickState = 0;
}

//---------------------------------------------------------------------------
// 重新開始遊戲.
//---------------------------------------------------------------------------
void resetGame()
{
	// 遊戲狀態.
	gameStatus = 0;

	// 下降速度.
	downSpeed = 1000;
	downSpeedTemp = 0;

	// 亂數產生方塊.
	brickId = (byte)random(1, 8);
	// 方塊狀態(0~3).
	brickState = 0;

	// 方塊在容器的位置.
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
	
	// 清除方塊數量(暫停).
	clearBrickFlag = 1;

	// 清除容器.
	for (int iy = 0; iy < 8; iy++) {
		for (int ix = 0; ix < 8; ix++) {
			container[ix][iy] = 0;
		}
	}	
}