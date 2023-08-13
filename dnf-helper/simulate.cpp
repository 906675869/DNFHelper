#include "simulate.h"
#include "call.h"
#include "common.h"
#include "keyboard.h"
#include "judge.h"

void ReleaseAllKeys()
{
	if (kb.IsPressed(左光标键)) {
		kb.Press(左光标键, 4);
		Sleep(10);
	}
	if (kb.IsPressed(右光标键)) {
		kb.Press(右光标键, 4);
		Sleep(10);
	}
	if (kb.IsPressed(上光标键)) {
		kb.Press(上光标键, 4);
		Sleep(10);
	}
	if (kb.IsPressed(下光标键)) {
		kb.Press(下光标键, 4);
		Sleep(10);
	}
}

void ReleaseXKeys() 
{
	if (kb.IsPressed(左光标键)) {
		kb.Press(左光标键, 4);
		Sleep(10);
	}
	if (kb.IsPressed(右光标键)) {
		kb.Press(右光标键, 4);
		Sleep(10);
	}
}

void ReleaseYKeys()
{
	if (kb.IsPressed(上光标键)) {
		kb.Press(上光标键, 4);
		Sleep(10);
	}
	if (kb.IsPressed(下光标键)) {
		kb.Press(下光标键, 4);
		Sleep(10);
	}
}

// 模拟到达指定坐标
void Simulate::GoDestation(int x, int y, CoordinateStruct beforeCoordinate, bool findGoods)
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
	if (findGoods) 
	{
		offset = 10;
	}

	while (gd.autoSwitch) {
		// 容错
		if (loopCnt++ > 200) {
			ReleaseAllKeys();
			break;
		}
		if (!jd.IsAtMap()) {
			ReleaseAllKeys();
			break;
		}
		// 获取当前房间坐标
		CoordinateStruct currentCoordinate = jd.GetCurrentRoom();
		if (!jd.CoordinateEqual(beforeCoordinate, currentCoordinate)) {
			ReleaseAllKeys();
			break;
		}
		// 当前为boss房间 不执行过图
		CoordinateStruct bossCoordinate = jd.GetBossRoom();
		if (jd.CoordinateEqual(bossCoordinate, currentCoordinate) && jd.IsPassMap()) {
			ReleaseAllKeys();
			break;
		}

		CoordinateStruct rwCoordinate = cl.ReadCoordinate(gd.personPtr);
		// 到达目标地址
		if (x - offset < rwCoordinate.x && rwCoordinate.x < x + offset && y - offset < rwCoordinate.y && rwCoordinate.y < y + offset)
		{
			ReleaseAllKeys();
			if (x - offset < rwCoordinate.x) {
				kb.Press(左光标键);
			}
			if (rwCoordinate.x < x + offset) {
				kb.Press(右光标键);
			}
			break;
		}

		if (x - offset < rwCoordinate.x && rwCoordinate.x < x + offset) {
			ReleaseXKeys();
			Sleep(50);
		}

		if (y - offset < rwCoordinate.y && rwCoordinate.y < y + offset) {
			ReleaseYKeys();
			Sleep(50);
		}

		if (rwCoordinate.x > x + offset) {
			if (kb.IsPressed(右光标键)) {
				kb.Press(右光标键, 4);
			}
			// 未按下
			if (!kb.IsPressed(左光标键)) {
				Sleep(50);
				kb.Press(左光标键);
				// kb.Press(左光标键);
				Sleep(20);
				kb.Press(左光标键, 3);
				Sleep(20);
				kb.Press(左光标键, 3);
				// Sleep(abs(rwCoordinate.x - x) / 1163 * 1000);
			}

		}
		if (rwCoordinate.x < x - offset) {
			if (kb.IsPressed(左光标键)) {
				kb.Press(左光标键, 4);
			}
			if (!kb.IsPressed(右光标键)) {
				Sleep(50);
				kb.Press(右光标键);
				// kb.Press(右光标键);
				Sleep(20);
				kb.Press(右光标键, 3);
				Sleep(20);
				kb.Press(右光标键, 3);
				// Sleep(abs(rwCoordinate.x - x) / 1163 * 1000);
			}
		}
		if (rwCoordinate.y > y + offset) {
			if (kb.IsPressed(下光标键)) {
				kb.Press(下光标键, 4);
			}
			if (!kb.IsPressed(上光标键)) {
				kb.Press(上光标键, 3);
			}
		}
		if (rwCoordinate.y < y - offset) {
			if (kb.IsPressed(上光标键)) {
				kb.Press(上光标键, 4);
			}
			if (!kb.IsPressed(下光标键)) {
				kb.Press(下光标键, 3);
			}
		}
		Sleep(20);

	}

}




