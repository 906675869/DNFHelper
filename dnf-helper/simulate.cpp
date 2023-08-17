#include "simulate.h"
#include "call.h"
#include "common.h"
#include "keyboard.h"
#include "judge.h"

void ReleaseAllKeys()
{
	if (kb.IsPressed(左光标键)) {
		mskKb.KeyUp(左光标键);
		Sleep(10);
	}
	if (kb.IsPressed(右光标键)) {
		mskKb.KeyUp(右光标键);
		Sleep(10);
	}
	if (kb.IsPressed(上光标键)) {
		mskKb.KeyUp(上光标键);
		Sleep(10);
	}
	if (kb.IsPressed(下光标键)) {
		mskKb.KeyUp(下光标键);
		Sleep(10);
	}
}

void ReleaseXKeys() 
{
	if (kb.IsPressed(左光标键)) {
		mskKb.KeyUp(左光标键);
		Sleep(10);
	}
	if (kb.IsPressed(右光标键)) {
		mskKb.KeyUp(右光标键);
		Sleep(10);
	}
}

void ReleaseYKeys()
{
	if (kb.IsPressed(上光标键)) {
		mskKb.KeyUp(上光标键);
		Sleep(10);
	}
	if (kb.IsPressed(下光标键)) {
		mskKb.KeyUp(下光标键);
		Sleep(10);
	}
}

// 模拟到达指定坐标
void Simulate::GoDestation(int x, int y, CoordinateStruct beforeCoordinate, bool lowOffset)
{
	int loopCnt = 0;
	if (beforeCoordinate.x == -1)
	{
		beforeCoordinate = jd.GetCurrentRoom();
	}
	if (x < 0 || y < 0)
	{
		return;
	}
	int offset = 60;
	if (lowOffset)
	{
		offset = 10;
	}
	CoordinateStruct rwCoordinate = {0, 0, 0};

	while (gd.autoSwitch) {
		// 容错
		if (loopCnt++ > 250) {
			ReleaseAllKeys();
			break;
		}
		if (!jd.IsAtMap()) {
			ReleaseAllKeys();
			break;
		}
		// 获取当前房间坐标
		CoordinateStruct currentCoordinate = jd.GetCurrentRoom();
		// 已过图
		if (!jd.CoordinateEqual(beforeCoordinate, currentCoordinate)) {
			ReleaseAllKeys();
			break;
		}
		// 当前为boss房间 不执行过图
		//CoordinateStruct bossCoordinate = jd.GetBossRoom();
		//if (jd.CoordinateEqual(bossCoordinate, currentCoordinate) && jd.IsPassMap()) {
			//ReleaseAllKeys();
			//break;
		//}
		CoordinateStruct personCoordinate = cl.ReadCoordinate(gd.personPtr);
		// 比较上次按键的人物坐标和当前坐标
		if (rwCoordinate.x && rwCoordinate.y && jd.CoordinateEqual(rwCoordinate, personCoordinate))
		{
			// 未移动人物，可能按键卡死
			ReleaseAllKeys();
		}
		// 执行按键前获取一次坐标
		rwCoordinate = personCoordinate;
		// 到达目标地址
		if (x - offset < rwCoordinate.x && rwCoordinate.x < x + offset && y - offset/3 < rwCoordinate.y && rwCoordinate.y < y + offset/3)
		{
			ReleaseAllKeys();
			if (x - offset < rwCoordinate.x) {
				mskKb.Press(左光标键);
			}
			if (rwCoordinate.x < x + offset) {
				mskKb.Press(右光标键);
			}
			break;
		}

		if (x - offset < rwCoordinate.x && rwCoordinate.x < x + offset) {
			ReleaseXKeys();
		}

		if (y - offset/3 < rwCoordinate.y && rwCoordinate.y < y + offset/3) {
			ReleaseYKeys();
		}

		if (rwCoordinate.x > x + offset) {
			if (kb.IsPressed(右光标键)) {
				mskKb.KeyUp(右光标键);
				Sleep(5);
			}
			// 未按下
			if (!kb.IsPressed(左光标键) || jd.GetPersonAction() != 14) {
				mskKb.Press(左光标键);
				Sleep(10);
				mskKb.KeyDown(左光标键);
				Sleep(10);
			}
		}
		if (rwCoordinate.x < x - offset) {
			if (kb.IsPressed(左光标键)) {
				mskKb.KeyUp(左光标键);
				Sleep(5);
			}
			if (!kb.IsPressed(右光标键) || jd.GetPersonAction() != 14) {
				mskKb.Press(右光标键);
				Sleep(10);
				mskKb.KeyDown(右光标键);
				Sleep(10);
			}
		}
		if (rwCoordinate.y > y + offset/3) {
			if (kb.IsPressed(下光标键)) {
				mskKb.KeyUp(下光标键);
				Sleep(10);
			}
			if (!kb.IsPressed(上光标键)) {
				mskKb.KeyDown(上光标键);
				Sleep(10);
			}
		}
		if (rwCoordinate.y < y - offset/3) {
			if (kb.IsPressed(上光标键)) {
				mskKb.KeyUp(上光标键);
				Sleep(10);
			}
			if (!kb.IsPressed(下光标键)) {
				mskKb.KeyDown(下光标键);
				Sleep(10);
			}
		}
		
		Sleep(10);
		
	}

}




