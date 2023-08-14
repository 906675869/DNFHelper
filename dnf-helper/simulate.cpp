#include "simulate.h"
#include "call.h"
#include "common.h"
#include "keyboard.h"
#include "judge.h"

void ReleaseAllKeys()
{
	if (kb.IsPressed(�����)) {
		kb.Press(�����, 4);
		Sleep(10);
	}
	if (kb.IsPressed(�ҹ���)) {
		kb.Press(�ҹ���, 4);
		Sleep(10);
	}
	if (kb.IsPressed(�Ϲ���)) {
		kb.Press(�Ϲ���, 4);
		Sleep(10);
	}
	if (kb.IsPressed(�¹���)) {
		kb.Press(�¹���, 4);
		Sleep(10);
	}
}

void ReleaseXKeys() 
{
	if (kb.IsPressed(�����)) {
		kb.Press(�����, 4);
		Sleep(10);
	}
	if (kb.IsPressed(�ҹ���)) {
		kb.Press(�ҹ���, 4);
		Sleep(10);
	}
}

void ReleaseYKeys()
{
	if (kb.IsPressed(�Ϲ���)) {
		kb.Press(�Ϲ���, 4);
		Sleep(10);
	}
	if (kb.IsPressed(�¹���)) {
		kb.Press(�¹���, 4);
		Sleep(10);
	}
}

// ģ�⵽��ָ������
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
		// �ݴ�
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
		if (jd.CoordinateEqual(bossCoordinate, currentCoordinate) && jd.IsPassMap()) {
			ReleaseAllKeys();
			break;
		}

		CoordinateStruct rwCoordinate = cl.ReadCoordinate(gd.personPtr);
		// ����Ŀ���ַ
		if (x - offset < rwCoordinate.x && rwCoordinate.x < x + offset && y - offset < rwCoordinate.y && rwCoordinate.y < y + offset)
		{
			ReleaseAllKeys();
			if (x - offset < rwCoordinate.x) {
				kb.Press(�����);
			}
			if (rwCoordinate.x < x + offset) {
				kb.Press(�ҹ���);
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
				// Sleep(abs(rwCoordinate.x - x) / 1163 * 1000);
			}

		}
		if (rwCoordinate.x < x - offset) {
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
				// Sleep(abs(rwCoordinate.x - x) / 1163 * 1000);
			}
		}
		if (rwCoordinate.y > y + offset) {
			if (kb.IsPressed(�¹���)) {
				kb.Press(�¹���, 4);
			}
			if (!kb.IsPressed(�Ϲ���)) {
				kb.Press(�Ϲ���, 3);
			}
		}
		if (rwCoordinate.y < y - offset) {
			if (kb.IsPressed(�Ϲ���)) {
				kb.Press(�Ϲ���, 4);
			}
			if (!kb.IsPressed(�¹���)) {
				kb.Press(�¹���, 3);
			}
		}
		Sleep(20);

	}

}




