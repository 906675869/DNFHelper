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

// 开启自动
void Auto::AutoSwitch()
{
	gd.autoSwitch = !gd.autoSwitch;
	if (gd.autoSwitch) {
		cout << "开启自动刷图" << endl;
	}
	else {
		cout << "关闭自动刷图" << endl;
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
		cout << "为防止刷图过快三方导致行为判定，暂时休眠20~30秒";
		Sleep(1000 * GetRandomNum(20, 30));
	}
	// 进图处理
	if (config.ReadConfigItem(configData.autoModel) == 1) { // 指定地图
		jd.GetMapLevel();
	}
	if (config.ReadConfigItem(configData.autoModel) == 2) {// 剧情
		gd.mapId = tk.HandleMain();
		gd.mapLevel = 0;
	}
	cl.AreaCall(gd.mapId);
	Sleep(200);
	pk.ChooseMap();
	Sleep(50);
	pk.GetinMap(gd.mapId, gd.mapLevel, NULL, NULL);
	// 延迟至进图
	while (gd.autoSwitch) {
		if (jd.IsAtMap() ||jd.IsAtTown())
		{
			break;
		}
		Sleep(200);
		kb.Press(空格键);
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
		at.IgnoreBuildings(true);
		if (config.ReadConfigItem(configData.inviciable) == 1) {
			at.Invincible();
		}
	}
	// 已通关
	if (jd.IsBossRoom() && jd.IsPassMap()) {
		cout << "执行通关处理" << endl;
		ClearMap();
		firstEnterMap = true;
		as.overMapCnt = 0;
		at.IgnoreBuildings(false);
	}
	// 未开门
	if (!jd.IsOpenDoor())
	{
		// 跟随怪物
		at.FollowMonster();
	}
	// 已开门
	else if(!jd.IsBossRoom()){
		// 循环捡物
		if (config.ReadConfigItem(configData.pickupType) == 1) {
			at.CoordinatePickUp();
		}
		if (config.ReadConfigItem(configData.pickupType) == 2) {
			at.PackPickUp();
		}
		// 过图
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
		// 成功过图
		if (!jd.IsAtMap() || !jd.CoordinateEqual(beforeCoorinate, currentCoorinate) ) {
			as.overMapCnt = 0;
			break;
		}
		
	}
}

void Auto::ClearMap()
{
	at.CoordinatePickUp();
	cout << "执行翻牌处理" << endl;
	// 随机翻牌
	pk.FlopCard(0, GetRandomNum(0, 3));
	int st = GetRandomNum(5000, 8000);
	Sleep(st);
	kb.Press(Esc键);
	// 判断是否达到预设的疲劳值
	if (jd.GetFatigue() < config.ReadConfigItem(configData.leftFatigue)) {
		// 返回城镇
		cout << "执行返回城镇" << endl;
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
		kb.Press(Esc键);
		Sleep(200);
		kb.Press(Esc键);
		// 重新挑战
		cout << "执行重新挑战" << endl;
		kb.Press(F10键);
	}
	else {
		kb.Press(Esc键);
		Sleep(200);
		kb.Press(Esc键);
		cout << "执行返回城镇" << endl;
		kb.Press(F12键);
		
	}
	int passMapCnt = 0;
	while (gd.autoSwitch) {
		// 在城镇或者重新开始通关
		if (jd.IsAtTown() || (!jd.IsBossRoom() && !jd.IsPassMap())) {
			break;
		}
		if (passMapCnt++ ==  50 * 5) {
			if (continueMap)
			{
				// 重新挑战
				cout << "强制执行重新挑战" << endl;
				pk.ContinueChangle();
			}
			else
			{
				// 返回城镇
				cout << "强制执行返回城镇" << endl;
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
		cout << "完成指定角色数量" << endl;
		gd.autoSwitch = FALSE;
		return;
	}
	Sleep(200);
	pk.ChooseRole(roleNum);
	Sleep(500);
	cout << "进入角色:" << as.chooseRoleNum + 1 <<endl;
	
	while (gd.autoSwitch) {
		if (jd.IsAtTown()) {
			break;
		}
		Sleep(200);
	}


}
