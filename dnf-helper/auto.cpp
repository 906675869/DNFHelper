#include "auto.h"
#include "common.h"
#include <iostream>
#include "judge.h"
#include "call.h"
#include "pack.h"
#include "action.h"
#include "pass_map.h"
#include "keyboard.h"
#include "helper.h"

static bool firstEnterMap = true;

// �����Զ�
void Auto::AutoSwitch()
{
	gd.autoSwitch = !gd.autoSwitch;
	if (gd.autoSwitch) {
		cout << "�����Զ�ˢͼ" << endl;
	}
	else {
		cout << "�ر��Զ�ˢͼ" << endl;
	}
	while (gd.autoSwitch)
	{
		if (jd.IsAtTown()) 
		{
			// cout << "��ǰ�ڳ���" << endl;
			TownProcess();
		}
		else if (jd.IsAtMap())
		{
			// cout << "��ǰ�ڵ�ͼ" << endl;
			EveryRoomLoop();
		}

		Sleep(200);
	}
}

void Auto::TownProcess()
{
	// ��ͼ����
	jd.GetMapLevel();
	cl.AreaCall(gd.mapId);
	Sleep(200);
	pk.ChooseMap();
	Sleep(50);
	pk.GetinMap(gd.mapId, gd.mapLevel, NULL, NULL);
	// �ӳ�����ͼ
	while (gd.autoSwitch) {
		if (jd.IsAtMap())
		{
			break;
		}
		Sleep(200);
	}
}


void Auto::EveryRoomLoop()
{
	// ���ڵ�ͼ��
	if (!jd.IsAtMap()) 
	{
		return;
	}
	if (firstEnterMap) {
		firstEnterMap = false;
		at.IgnoreBuildings(true);
	}
	// ��ͨ��
	if (jd.IsBossRoom() && jd.IsPassMap()) {
		cout << "ִ��ͨ�ش���" << endl;
		ClearMap();
		firstEnterMap = true;
		as.overMapCnt = 0;
		at.IgnoreBuildings(false);
	}
	// δ����
	if (!jd.IsOpenDoor())
	{
		// �������
		at.FollowMonster();
	}
	// �ѿ���
	else if(!jd.IsBossRoom()){
		// ѭ������
		at.PackPickUp();
		// ��ͼ
		OverMap();
	}
	
	
}

void Auto::OverMap()
{
	CoordinateStruct beforeCoorinate = jd.GetCurrentRoom();
	while (gd.autoSwitch) {
		as.overMapCnt++;
		pm.FixFindRoute();
		Sleep(300);
		CoordinateStruct  currentCoorinate = jd.GetCurrentRoom();
		// �ɹ���ͼ
		if (!jd.IsAtMap() || !jd.CoordinateEqual(beforeCoorinate, currentCoorinate) ) {
			as.overMapCnt = 0;
			break;
		}
		
	}
}

void Auto::ClearMap()
{
	at.PackPickUp();
	cout << "ִ�з��ƴ���" << endl;
	pk.FlopCard(0, GetRandomNum(0, 3));
	int st = GetRandomNum(5000, 8000);
	Sleep(st);
	kb.Press(Esc��);
	if (GetRandomNum(0, 10) % 5 < 2)
	{
		// ������ս
		cout << "ִ��������ս" << endl;
		pk.ContinueChangle();
	}
	else
	{
		// ���س���
		cout << "ִ�з��س���" << endl;
		pk.OutMap();
	}
	while (gd.autoSwitch) {
		// �ڳ���������¿�ʼͨ��
		if (jd.IsAtTown() || (!jd.IsBossRoom() && !jd.IsPassMap())) {
			break;
		}
		Sleep(20);
	}
	
}
