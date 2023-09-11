#include "simulate.h"
#include "call.h"
#include "common.h"
#include "keyboard.h"
#include "judge.h"

void ReleaseAllKeys()
{
	if (kb.IsPressed(�����)) {
		mskKb.KeyUp(�����);
		Sleep(10);
	}
	if (kb.IsPressed(�ҹ���)) {
		mskKb.KeyUp(�ҹ���);
		Sleep(10);
	}
	if (kb.IsPressed(�Ϲ���)) {
		mskKb.KeyUp(�Ϲ���);
		Sleep(10);
	}
	if (kb.IsPressed(�¹���)) {
		mskKb.KeyUp(�¹���);
		Sleep(10);
	}
}

void ReleaseXKeys() 
{
	if (kb.IsPressed(�����)) {
		mskKb.KeyUp(�����);
		Sleep(10);
	}
	if (kb.IsPressed(�ҹ���)) {
		mskKb.KeyUp(�ҹ���);
		Sleep(10);
	}
}

void ReleaseYKeys()
{
	if (kb.IsPressed(�Ϲ���)) {
		mskKb.KeyUp(�Ϲ���);
		Sleep(10);
	}
	if (kb.IsPressed(�¹���)) {
		mskKb.KeyUp(�¹���);
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
		if (loopCnt++ > 250) {
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
		if (x - offset < rwCoordinate.x && rwCoordinate.x < x + offset && y - offset/3 < rwCoordinate.y && rwCoordinate.y < y + offset/3)
		{
			ReleaseAllKeys();
			if (x - offset < rwCoordinate.x) {
				mskKb.Press(�����);
			}
			if (rwCoordinate.x < x + offset) {
				mskKb.Press(�ҹ���);
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
			if (kb.IsPressed(�ҹ���)) {
				mskKb.KeyUp(�ҹ���);
				Sleep(5);
			}
			// δ����
			if (!kb.IsPressed(�����) || jd.GetPersonAction() != 14) {
				mskKb.Press(�����);
				Sleep(10);
				mskKb.KeyDown(�����);
				Sleep(10);
			}
		}
		if (rwCoordinate.x < x - offset) {
			if (kb.IsPressed(�����)) {
				mskKb.KeyUp(�����);
				Sleep(5);
			}
			if (!kb.IsPressed(�ҹ���) || jd.GetPersonAction() != 14) {
				mskKb.Press(�ҹ���);
				Sleep(10);
				mskKb.KeyDown(�ҹ���);
				Sleep(10);
			}
		}
		if (rwCoordinate.y > y + offset/3) {
			if (kb.IsPressed(�¹���)) {
				mskKb.KeyUp(�¹���);
				Sleep(10);
			}
			if (!kb.IsPressed(�Ϲ���)) {
				mskKb.KeyDown(�Ϲ���);
				Sleep(10);
			}
		}
		if (rwCoordinate.y < y - offset/3) {
			if (kb.IsPressed(�Ϲ���)) {
				mskKb.KeyUp(�Ϲ���);
				Sleep(10);
			}
			if (!kb.IsPressed(�¹���)) {
				mskKb.KeyDown(�¹���);
				Sleep(10);
			}
		}
		
		Sleep(10);
		
	}

}




