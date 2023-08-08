#include "simulate.h"
#include "call.h"
#include "common.h"
#include "keyboard.h"
#include "judge.h"

void ReleaseAllKeys()
{
	if (kb.IsPressed(左光标键)) {
		kb.Press(左光标键, 4);
	}
	if (kb.IsPressed(右光标键)) {
		kb.Press(右光标键, 4);
	}
	if (kb.IsPressed(上光标键)) {
		kb.Press(上光标键, 4);
	}
	if (kb.IsPressed(下光标键)) {
		kb.Press(下光标键, 4);
	}
}

void ReleaseXKeys() 
{
	if (kb.IsPressed(左光标键)) {
		kb.Press(左光标键, 4);
	}
	if (kb.IsPressed(右光标键)) {
		kb.Press(右光标键, 4);
	}
}

void ReleaseYKeys()
{
	if (kb.IsPressed(上光标键)) {
		kb.Press(上光标键, 4);
	}
	if (kb.IsPressed(下光标键)) {
		kb.Press(下光标键, 4);
	}
}

// 模拟到达指定坐标
void Simulate::GoDestation(int x, int y, CoordinateStruct beforeCoordinate)
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
	while (gd.autoSwitch) {
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
		if (jd.CoordinateEqual(bossCoordinate, currentCoordinate)) {
			ReleaseAllKeys();
			break;
		}

		CoordinateStruct rwCoordinate = cl.ReadCoordinate(gd.personPtr);
		// 到达目标地址
		if (x - 30 < rwCoordinate.x && rwCoordinate.x < x + 30 && y - 30 < rwCoordinate.y && rwCoordinate.y < y + 30)
		{
			ReleaseAllKeys();
			break;
		}

		if (x - 30 < rwCoordinate.x && rwCoordinate.x < x + 30) {
			ReleaseXKeys();
			Sleep(50);
		}

		if (y - 30 < rwCoordinate.y && rwCoordinate.y < y + 30) {
			ReleaseYKeys();
			Sleep(50);
		}

		if (rwCoordinate.x > x + 30) {
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
				Sleep(abs(rwCoordinate.x - x) / 1163 * 1000);
			}

		}
		if (rwCoordinate.x < x - 30) {
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
				Sleep(abs(rwCoordinate.x - x) / 1163 * 1000);
			}
		}
		if (rwCoordinate.y > y + 30) {
			if (kb.IsPressed(下光标键)) {
				kb.Press(下光标键, 4);
			}
			if (!kb.IsPressed(上光标键)) {
				kb.Press(上光标键, 3);
			}
		}
		if (rwCoordinate.y < y - 30) {
			if (kb.IsPressed(上光标键)) {
				kb.Press(上光标键, 4);
			}
			if (!kb.IsPressed(下光标键)) {
				kb.Press(下光标键, 3);
			}
		}
		Sleep(60);

	}

}




