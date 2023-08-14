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
		cout << ">> �����Զ�ˢͼ" << endl;
	}
	else {
		cout << ">> �ر��Զ�ˢͼ" << endl;
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

int townSleepFlag = false;

void Auto::TownProcess()
{
	Sleep(3000);
	// �ӳٽ�ͼ
	if (as.chooseRoleNum % 2 == 0) {
		townSleepFlag = false;
	}
	if (as.chooseRoleNum % 2 > 0 && !townSleepFlag) {
		cout << ">> Ϊ��ֹˢͼ��������������Ϊ�ж�����ʱ����3~5min";
		Sleep(60 * 1000 * GetRandomNum(3, 5));
		townSleepFlag = true;
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
		if (config.ReadConfigItem(configData.followModel) == 3) {
			at.IgnoreBuildings(true);
		}
		if (config.ReadConfigItem(configData.inviciable) == 1) {
			at.Invincible();
		}
		if (config.ReadConfigItem(configData.superBuff) == 1) {
			at.SuperSpecialBuff();
		}
		if (config.ReadConfigItem(configData.goodsBuff) == 1) {
			at.GoodsBuff();
		}
		// if(config.ReadConfigItem(configData.))
		at.SuperScore();
		// ����buff
		kb.Press(�ҹ���);
		Sleep(100);
		kb.Press(�ҹ���);
		Sleep(130);
		kb.Press(�ո��);
		Sleep(500);
		kb.Press(�ո��);

	}
	// ��ͨ��
	if (jd.IsBossRoom() && jd.IsPassMap()) {
		cout << ">> ִ��ͨ�ش���" << endl;
		ClearMap();
		firstEnterMap = true;
		as.overMapCnt = 0;
		// ����ͼ������
		if (config.ReadConfigItem(configData.followModel) == 3 && jd.IsAtTown()) {
			at.IgnoreBuildings(false);
		}
		
	}
	// δ����
	if (!jd.IsOpenDoor())
	{
		// �������
		at.FollowMonster();
		if (jd.IsDied()) {
			Sleep(3000);
			// ���ĸ����
			kb.Press(X��);
		}

	}
	// �ѿ���
	else if(!jd.IsBossRoom()){
		// ѭ������
		if (config.ReadConfigItem(configData.pickupType) == 1) {
			Sleep(2812);
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
	Sleep(1400);
	cout << "ִ�з��ƴ���" << endl;
	// �������
	// pk.FlopCard(0, GetRandomNum(0, 3));
	int st = GetRandomNum(5000, 8000);
	Sleep(st);
	kb.Press(�ȼ�);
	at.CoordinatePickUp();
	// kb.Press(Esc��);
	// �ж��Ƿ�ﵽԤ���ƣ��ֵ
	if (jd.GetFatigue() < config.ReadConfigItem(configData.leftFatigue)) {
		// ���س���
		cout << ">> ִ�з��س���" << endl;
		pk.OutMap();
		while (gd.autoSwitch) {
			Sleep(200);
			if (jd.IsAtTown()) {
				pk.RoleList();
			}
			if (jd.IsAtRoleList()) {
				break;
			}
		}
		return;
	}
	bool continueMap = GetRandomNum(0, 9) < 6;
	
	int passMapCnt = 0;
	while (gd.autoSwitch) {
		// �ڳ���������¿�ʼͨ��
		if (jd.IsAtTown() || (!jd.IsBossRoom() && !jd.IsPassMap())) {
			break;
		}
		if (passMapCnt++ ==  30) {
			if (continueMap)
			{
				// ������ս
				cout << ">> ǿ��ִ��������ս" << endl;
				pk.ContinueChangle();
			}
			else
			{
				// ���س���
				cout << ">> ǿ��ִ�з��س���" << endl;
				pk.OutMap();
			}
		}
		else {
			if (continueMap) {
				if (passMapCnt <= 3) {
					kb.Press(Esc��);
					Sleep(200);
				}
				// ������ս
				if(passMapCnt == 1) cout << ">> ִ��������ս" << endl;
				kb.FnPress(VK_F10);
			}
			else {
				if (passMapCnt <= 3) {
					kb.Press(Esc��);
					Sleep(200);
				}
				if (passMapCnt == 1) cout << ">> ִ�з��س���" << endl;
				kb.FnPress(VK_F12);

			}
		
		}
		Sleep(200);
	}
	
}

void Auto::EnterTown()
{
	int totalRoleNum = jd.GetRoleNum();
	int roleNum = config.ReadConfigItem(configData.roleNum);
	roleNum = min(totalRoleNum, roleNum);
	as.chooseRoleNum = as.chooseRoleNum + 1;
	if (as.chooseRoleNum > roleNum) {
		cout << ">> ���ָ����ɫ����,ֹͣˢͼ!" << endl;
		gd.autoSwitch = FALSE;
		return;
	}
	Sleep(200);
	kb.Press(�ҹ���);
	Sleep(500);
	kb.Press(�ո��);
	cout << ">> �����ɫ:" << as.chooseRoleNum + 1 <<endl;
	int dely = 0;
	while (gd.autoSwitch) {
		if (jd.IsAtTown()) {
			break;
		}
		if (dely++ == 30) {
			cout << ">> ǿ�ƽ����ɫ:" << as.chooseRoleNum + 1 << endl;
			pk.ChooseRole(roleNum);
		}
		Sleep(200);
	}


}
