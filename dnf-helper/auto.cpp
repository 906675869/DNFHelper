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
static UINT passMapCnt = 0;

// 开启自动
void Auto::AutoSwitch()
{
	gd.autoSwitch = !gd.autoSwitch;
	if (gd.autoSwitch) {
		cout << ">> 开启自动刷图" << endl;
	}
	else {
		cout << ">> 关闭自动刷图" << endl;
	}
	int choseMapCnt = 0;
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
		else if (jd.IsAtChooseMap()) 
		{
			// 选图容错
			if (choseMapCnt++ % 15 == 0) 
			{
				if (choseMapCnt / 15) 
				{
					for (int i = 0; i < choseMapCnt / 15; i++) 
					{
						kb.Press(左光标键);
						Sleep(200);
					}
				}
				kb.Press(空格键);
			}
		}
		else if (jd.IsAtMap())
		{
			choseMapCnt = 0;
			EveryRoomLoop();
		}

		Sleep(200);
	}
}

int townSleepFlag = false;

void Auto::TownProcess()
{
	Sleep(3000);
	// 延迟进图
	if (as.chooseRoleNum % 2 == 0) {
		townSleepFlag = false;
	}
	if (as.chooseRoleNum % 2 > 0 && !townSleepFlag) {
		cout << ">> 为防止刷图过快三方导致行为判定，暂时休眠3~5min";
		Sleep(60 * 1000 * GetRandomNum(3, 5));
		townSleepFlag = true;
	}
	// 进图处理
	if (config.ReadConfigItem(configData.autoModel) == 1) { // 指定地图
		jd.GetMapLevel();
	}
	if (config.ReadConfigItem(configData.autoModel) == 2) {// 剧情
		gd.mapId = tk.HandleMain();
		gd.mapLevel = 0;
	}

	if (jd.GetWeight() > 90)
	{
		at.EquipProcess();
	}


	cl.AreaCall(gd.mapId);
	Sleep(3200);
	pk.ChooseMap();
	Sleep(300);
	pk.GetinMap(gd.mapId, gd.mapLevel, NULL, NULL);
	int dely = 0;
	
	// 延迟至进图
	while (gd.autoSwitch) {
		if (jd.IsAtMap() ||jd.IsAtTown() || dely++ == 50)
		{
			break;
		}
		Sleep(200);
	}
}


void Auto::EveryRoomLoop()
{
	// 不在地图内
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
		// 按键buff
		kb.Press(右光标键);
		Sleep(100);
		kb.Press(右光标键);
		Sleep(130);
		kb.Press(空格键);
		Sleep(500);
		kb.Press(空格键);

	}
	// 跟随怪物
	at.FollowMonster();


	// 已通关
	if (jd.IsBossRoom() && jd.IsPassMap()) {
		cout << ">> 执行通关处理,已通关["<< passMapCnt ++ <<"]次" << endl;
		ClearMap();
		firstEnterMap = true;
		as.overMapCnt = 0;
		// 出地图后重置
		if (config.ReadConfigItem(configData.followModel) == 3 && jd.IsAtTown()) {
			at.IgnoreBuildings(false);
		}
		return;
		
	}

	// 已开门
	if(jd.IsOpenDoor() && !jd.IsBossRoom()){
		// 循环捡物
		if (config.ReadConfigItem(configData.pickupType) == 1 && !as.specialProcess) {
			// Sleep(512);
			at.CoordinatePickUp();
			// 容错
			if (jd.HasGoods() && as.pickUpCnt ++ < 20) 
			{
				return;
			}
			as.pickUpCnt = 0;

		}
		if (config.ReadConfigItem(configData.pickupType) == 2 || as.specialProcess) {
			at.PackPickUp();
			// 容错
			if (jd.HasGoods() && as.pickUpCnt++ < 3)
			{
				return;
			}
			as.pickUpCnt = 0;
		}
		// 过图
		OverMap();
	}
	
	
}

void Auto::OverMap()
{
	CoordinateStruct beforeCoorinate = jd.GetCurrentRoom();
	while (gd.autoSwitch) {
		// 未开门
		as.overMapCnt++;
		pm.FixFindRoute();
		Sleep(600);
		CoordinateStruct  currentCoorinate = jd.GetCurrentRoom();
		// 成功过图
		if (!jd.IsAtMap() || !jd.IsOpenDoor() || !jd.CoordinateEqual(beforeCoorinate, currentCoorinate) || jd.IsPassMap() ) {
			// 不在图内，未开门，已过图, 已通关
			as.overMapCnt = 0;
			as.specialProcess = false;
			break;
		}
		
	}
}

void Auto::ContinueMap(bool continueMap)
{
	int passMapCnt = 0;
	while (gd.autoSwitch) {
		// 在城镇或者重新开始通关
		if (jd.IsAtTown() || (!jd.IsBossRoom() && !jd.IsPassMap())) {
			break;
		}
		if (passMapCnt++ == 30) {
			if (continueMap)
			{
				// 重新挑战
				cout << ">> 强制执行重新挑战" << endl;
				pk.ContinueChangle();
			}
			else
			{
				// 返回城镇
				cout << ">> 强制执行返回城镇" << endl;
				pk.OutMap();
			}
		}
		else {
			if (continueMap) {
				if (passMapCnt <= 3) {
					kb.Press(Esc键);
					Sleep(200);
				}
				// 重新挑战
				if (passMapCnt == 1) cout << ">> 执行重新挑战" << endl;
				kb.FnPress(空格键);
				Sleep(500);
				if (passMapCnt > 5) {
					kb.FnPress(VK_F10);
				}
			}
			else {
				if (passMapCnt <= 3) {
					kb.Press(Esc键);
					Sleep(200);
				}
				if (passMapCnt == 1) cout << ">> 执行返回城镇" << endl;
				kb.FnPress(VK_F12);

			}

		}
		Sleep(200);
	}
}

void Auto::ClearMap()
{
	Sleep(400);
	kb.Press(等键);
	cout << "执行翻牌处理" << endl;
	// 随机翻牌
	// pk.FlopCard(0, GetRandomNum(0, 3));
	int st = GetRandomNum(5000, 8000);
	Sleep(st);
	at.CoordinatePickUp();
	int cnt = 0;
	while (jd.HasGoods()) {
		Sleep(200);
		at.CoordinatePickUp();
		if (cnt++ > 10) {
			break;
		}
	}
	// 减少负重
	if (jd.GetWeight() > 90)
	{
		at.EquipProcess();
	}

	// kb.Press(Esc键);
	// 判断是否达到预设的疲劳值
	if (jd.GetFatigue() < config.ReadConfigItem(configData.leftFatigue)) {
		// 返回城镇
		cout << ">> 执行返回城镇" << endl;
		ContinueMap(false);
		while (gd.autoSwitch) {
			Sleep(200);
			if (jd.IsAtTown()) {
				Sleep(3000);
				pk.RoleList();
			}
			if (jd.IsAtRoleList()) {
				break;
			}
		}
		return;
	}
	ContinueMap(GetRandomNum(0, 9) < 6);
	
}

void Auto::EnterTown()
{
	int totalRoleNum = jd.GetRoleNum();
	int roleNum = config.ReadConfigItem(configData.roleNum);
	roleNum = min(totalRoleNum, roleNum);
	as.chooseRoleNum = as.chooseRoleNum + 1;
	if (as.chooseRoleNum > roleNum) {
		cout << ">> 完成指定角色数量,停止刷图!" << endl;
		gd.autoSwitch = FALSE;
		return;
	}
	Sleep(200);
	kb.Press(右光标键);
	Sleep(500);
	kb.Press(空格键);
	cout << ">> 进入角色:" << as.chooseRoleNum + 1 <<endl;
	int dely = 0;
	while (gd.autoSwitch) {
		if (jd.IsAtTown()) {
			break;
		}
		if (dely++ == 30) {
			cout << ">> 强制进入角色:" << as.chooseRoleNum + 1 << endl;
			pk.ChooseRole(roleNum);
		}
		Sleep(200);
	}


}
