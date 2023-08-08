#include "simulate.h"
#include "call.h"
#include "common.h"
#include "keyboard.h"
#include "judge.h"

void ReleaseAllKeys()
{
	if (kb.IsPressed(�����)) {
		kb.Press(�����, 4);
	}
	if (kb.IsPressed(�ҹ���)) {
		kb.Press(�ҹ���, 4);
	}
	if (kb.IsPressed(�Ϲ���)) {
		kb.Press(�Ϲ���, 4);
	}
	if (kb.IsPressed(�¹���)) {
		kb.Press(�¹���, 4);
	}
}

void ReleaseXKeys() 
{
	if (kb.IsPressed(�����)) {
		kb.Press(�����, 4);
	}
	if (kb.IsPressed(�ҹ���)) {
		kb.Press(�ҹ���, 4);
	}
}

void ReleaseYKeys()
{
	if (kb.IsPressed(�Ϲ���)) {
		kb.Press(�Ϲ���, 4);
	}
	if (kb.IsPressed(�¹���)) {
		kb.Press(�¹���, 4);
	}
}

// ģ�⵽��ָ������
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
		// ��ȡ��ǰ��������
		CoordinateStruct currentCoordinate = jd.GetCurrentRoom();
		if (!jd.CoordinateEqual(beforeCoordinate, currentCoordinate)) {
			ReleaseAllKeys();
			break;
		}
		// ��ǰΪboss���� ��ִ�й�ͼ
		CoordinateStruct bossCoordinate = jd.GetBossRoom();
		if (jd.CoordinateEqual(bossCoordinate, currentCoordinate)) {
			ReleaseAllKeys();
			break;
		}

		CoordinateStruct rwCoordinate = cl.ReadCoordinate(gd.personPtr);
		// ����Ŀ���ַ
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
			if (kb.IsPressed(�ҹ���)) {
				kb.Press(�ҹ���, 4);
			}
			// δ����
			if (!kb.IsPressed(�����)) {
				Sleep(50);
				kb.Press(�����);
				// kb.Press(�����);
				Sleep(20);
				kb.Press(�����, 3);
				Sleep(20);
				kb.Press(�����, 3);
				Sleep(abs(rwCoordinate.x - x) / 1163 * 1000);
			}

		}
		if (rwCoordinate.x < x - 30) {
			if (kb.IsPressed(�����)) {
				kb.Press(�����, 4);
			}
			if (!kb.IsPressed(�ҹ���)) {
				Sleep(50);
				kb.Press(�ҹ���);
				// kb.Press(�ҹ���);
				Sleep(20);
				kb.Press(�ҹ���, 3);
				Sleep(20);
				kb.Press(�ҹ���, 3);
				Sleep(abs(rwCoordinate.x - x) / 1163 * 1000);
			}
		}
		if (rwCoordinate.y > y + 30) {
			if (kb.IsPressed(�¹���)) {
				kb.Press(�¹���, 4);
			}
			if (!kb.IsPressed(�Ϲ���)) {
				kb.Press(�Ϲ���, 3);
			}
		}
		if (rwCoordinate.y < y - 30) {
			if (kb.IsPressed(�Ϲ���)) {
				kb.Press(�Ϲ���, 4);
			}
			if (!kb.IsPressed(�¹���)) {
				kb.Press(�¹���, 3);
			}
		}
		Sleep(60);

	}

}




