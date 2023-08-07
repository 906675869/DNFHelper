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
#include "config.h"
#include "task.h"

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
		if (jd.IsAtRoleList()) 
		{
			EnterTown();
		}
		else if (jd.IsAtTown()) 
		{
			TownProcess();
		}
		else if (jd.IsAtMap())
		{
			EveryRoomLoop();
		}

		Sleep(200);
	}
}

void Auto::TownProcess()
{
	Sleep(3000);
	if (as.chooseRoleNum % 2 > 0) {
		cout << "Ϊ��ֹˢͼ��������������Ϊ�ж�����ʱ����20~30��";
		Sleep(1000 * GetRandomNum(20, 30));
	}
	// ��ͼ����
	if (config.ReadConfigItem(configData.autoModel) == 1) { // ָ����ͼ
		jd.GetMapLevel();
	}
	if (config.ReadConfigItem(configData.autoModel) == 2) {// ����
		gd.mapId = tk.HandleMain();
		gd.mapLevel = 0;
	}
	cl.AreaCall(gd.mapId);
	Sleep(200);
	pk.ChooseMap();
	Sleep(50);
	pk.GetinMap(gd.mapId, gd.mapLevel, NULL, NULL);
	// �ӳ�����ͼ
	while (gd.autoSwitch) {
		if (jd.IsAtMap() ||jd.IsAtTown())
		{
			break;
		}
		Sleep(200);
		kb.Press(�ո��);
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
		if (config.ReadConfigItem(configData.inviciable) == 1) {
			at.Invincible();
		}
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
		if (config.ReadConfigItem(configData.pickupType) == 1) {
			at.CoordinatePickUp();
		}
		if (config.ReadConfigItem(configData.pickupType) == 2) {
			at.PackPickUp();
		}
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
	at.CoordinatePickUp();
	cout << "ִ�з��ƴ���" << endl;
	// �������
	pk.FlopCard(0, GetRandomNum(0, 3));
	int st = GetRandomNum(5000, 8000);
	Sleep(st);
	kb.Press(Esc��);
	// �ж��Ƿ�ﵽԤ���ƣ��ֵ
	if (jd.GetFatigue() < config.ReadConfigItem(configData.leftFatigue)) {
		// ���س���
		cout << "ִ�з��س���" << endl;
		pk.OutMap();
		while (gd.autoSwitch) {
			Sleep(200);
			if (jd.IsAtTown()) {
				pk.RoleList();
			}
		}
		return;
	}
	bool continueMap = GetRandomNum(0, 10) % 5 < 3;
	if (continueMap) {
		kb.Press(Esc��);
		Sleep(200);
		kb.Press(Esc��);
		// ������ս
		cout << "ִ��������ս" << endl;
		kb.Press(F10��);
	}
	else {
		kb.Press(Esc��);
		Sleep(200);
		kb.Press(Esc��);
		cout << "ִ�з��س���" << endl;
		kb.Press(F12��);
		
	}
	int passMapCnt = 0;
	while (gd.autoSwitch) {
		// �ڳ���������¿�ʼͨ��
		if (jd.IsAtTown() || (!jd.IsBossRoom() && !jd.IsPassMap())) {
			break;
		}
		if (passMapCnt++ ==  50 * 5) {
			if (continueMap)
			{
				// ������ս
				cout << "ǿ��ִ��������ս" << endl;
				pk.ContinueChangle();
			}
			else
			{
				// ���س���
				cout << "ǿ��ִ�з��س���" << endl;
				pk.OutMap();
			}
		}
		Sleep(20);
	}
	
}

void Auto::EnterTown()
{
	int totalRoleNum = jd.GetRoleNum();
	int roleNum = config.ReadConfigItem(configData.roleNum);
	roleNum = min(totalRoleNum, roleNum);
	as.chooseRoleNum = as.chooseRoleNum + 1;
	if (as.chooseRoleNum > roleNum) {
		cout << "���ָ����ɫ����" << endl;
		gd.autoSwitch = FALSE;
		return;
	}
	Sleep(200);
	pk.ChooseRole(roleNum);
	Sleep(500);
	cout << "�����ɫ:" << as.chooseRoleNum + 1 <<endl;
	
	while (gd.autoSwitch) {
		if (jd.IsAtTown()) {
			break;
		}
		Sleep(200);
	}


}
