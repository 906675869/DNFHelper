#include "simulate.h"
#include "call.h"
#include "common.h"
#include "keyboard.h"
#include "judge.h"


// 模拟到达指定坐标
void Simulate::GoDestation(int x, int y, CoordinateStruct beforeCoordinate)
{
	int loopCnt = 0;
	bool left = false, right= false, down = false, up= false;
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
			kb.Press(左光标键, 4);
			kb.Press(右光标键, 4);
			kb.Press(上光标键, 4);
			kb.Press(下光标键, 4);
			break;
		}
		if (!jd.IsAtMap()) {
			kb.Press(左光标键, 4);
			kb.Press(右光标键, 4);
			kb.Press(上光标键, 4);
			kb.Press(下光标键, 4);
			break;
		}
		// 获取当前房间坐标
		CoordinateStruct currentCoordinate = jd.GetCurrentRoom();
		if (!jd.CoordinateEqual(beforeCoordinate, currentCoordinate)) {
			kb.Press(左光标键, 4);
			kb.Press(右光标键, 4);
			kb.Press(上光标键, 4);
			kb.Press(下光标键, 4);
			break;
		}
		// 当前为boss房间 不执行过图
		CoordinateStruct bossCoordinate = jd.GetBossRoom();
		if (jd.CoordinateEqual(bossCoordinate, currentCoordinate)) {
			kb.Press(左光标键, 4);
			kb.Press(右光标键, 4);
			kb.Press(上光标键, 4);
			kb.Press(下光标键, 4);
			break;
		}

		CoordinateStruct rwCoordinate = cl.ReadCoordinate(gd.personPtr);
		// 到达目标地址
		if (x - 30 < rwCoordinate.x && rwCoordinate.x < x + 30 && y - 30 < rwCoordinate.y && rwCoordinate.y < y + 30)
		{
			kb.Press(左光标键, 4);
			kb.Press(右光标键, 4);
			kb.Press(上光标键, 4);
			kb.Press(下光标键, 4);
			break;
		}

		if (x - 30 < rwCoordinate.x && rwCoordinate.x < x + 30) {
			kb.Press(左光标键, 4);
			kb.Press(右光标键, 4);
			left = false; right = false;
			Sleep(50);
		}

		if (y - 30 < rwCoordinate.y && rwCoordinate.y < y + 30) {
			kb.Press(上光标键, 4);
			kb.Press(下光标键, 4);
			up = false; down = false;
			Sleep(50);
		}

		if (rwCoordinate.x > x + 30) {
			if (right) {
				kb.Press(右光标键, 4);
				right = false;
			}
			if (!left) {
				Sleep(50);
				kb.Press(左光标键);
				// kb.Press(左光标键);
				Sleep(20);
				kb.Press(左光标键, 3);
				Sleep(20);
				kb.Press(左光标键, 3);
				left = true;
				Sleep(abs(rwCoordinate.x - x) / 1163 * 1000);
			}

		}
		if (rwCoordinate.x < x - 30) {
			if (left) {
				kb.Press(左光标键, 4);
				left = false;
			}
			if (!right) {
				Sleep(50);
				kb.Press(右光标键);
				// kb.Press(右光标键);
				Sleep(20);
				kb.Press(右光标键, 3);
				Sleep(20);
				kb.Press(右光标键, 3);
				right = true;
				Sleep(abs(rwCoordinate.x - x) / 1163 * 1000);
			}
		}
		if (rwCoordinate.y > y + 30) {
			if (down) {
				kb.Press(下光标键, 4);
				down = false;
			}
			kb.Press(上光标键, 3);
			up = true;
		}
		if (rwCoordinate.y < y - 30) {
			if (up) {
				kb.Press(上光标键, 4);
				up = false;
			}
			kb.Press(下光标键, 3);
			down = true;
		}
		Sleep(60);

	}

}




