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
		// �ݴ�
		if (loopCnt++ > 500) {
			ReleaseAllKeys();
			break;
		}
		if (!jd.IsAtMap()) {
			ReleaseAllKeys();
			break;
		}
		// ��ȡ��ǰ��������
		CoordinateStruct currentCoordinate = jd.GetCurrentRoom();
		// �ѹ�ͼ
		if (!jd.CoordinateEqual(beforeCoordinate, currentCoordinate)) {
			ReleaseAllKeys();
			break;
		}
		// ��ǰΪboss���� ��ִ�й�ͼ
		//CoordinateStruct bossCoordinate = jd.GetBossRoom();
		//if (jd.CoordinateEqual(bossCoordinate, currentCoordinate) && jd.IsPassMap()) {
			//ReleaseAllKeys();
			//break;
		//}
		CoordinateStruct personCoordinate = cl.ReadCoordinate(gd.personPtr);
		// �Ƚ��ϴΰ�������������͵�ǰ����
		if (rwCoordinate.x && rwCoordinate.y && jd.CoordinateEqual(rwCoordinate, personCoordinate))
		{
			// δ�ƶ�������ܰ�������
			ReleaseAllKeys();
		}
		// ִ�а���ǰ��ȡһ������
		rwCoordinate = personCoordinate;
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
		}

		if (y - offset < rwCoordinate.y && rwCoordinate.y < y + offset) {
			ReleaseYKeys();
		}

		if (rwCoordinate.x > x + offset) {
			if (kb.IsPressed(�ҹ���)) {
				kb.Press(�ҹ���, 4);
				Sleep(5);
			}
			// δ����
			if (!kb.IsPressed(�����) || jd.GetPersonAction() != 14) {
				kb.Press(�����);
				Sleep(10);
				kb.Press(�����, 3);
				Sleep(10);
			}
		}
		if (rwCoordinate.x < x - offset) {
			if (kb.IsPressed(�����)) {
				kb.Press(�����, 4);
				Sleep(5);
			}
			if (!kb.IsPressed(�ҹ���) || jd.GetPersonAction() != 14) {
				kb.Press(�ҹ���);
				Sleep(10);
				kb.Press(�ҹ���, 3);
				Sleep(10);
			}
		}
		if (rwCoordinate.y > y + offset/2) {
			if (kb.IsPressed(�¹���)) {
				kb.Press(�¹���, 4);
				Sleep(10);
			}
			if (!kb.IsPressed(�Ϲ���)) {
				kb.Press(�Ϲ���, 3);
				Sleep(10);
			}
		}
		if (rwCoordinate.y < y - offset/2) {
			if (kb.IsPressed(�Ϲ���)) {
				kb.Press(�Ϲ���, 4);
				Sleep(10);
			}
			if (!kb.IsPressed(�¹���)) {
				kb.Press(�¹���, 3);
				Sleep(10);
			}
		}
		
		Sleep(20);
		
	}

}




