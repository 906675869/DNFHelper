#include "judge.h"
#include "config.h"
#include <iostream>
Judge jd;
// 获取角色疲劳
int Judge::GetFatigue()
{
	return rw.ReadInt(最大疲劳) - rw.ReadInt(当前疲劳);
}

bool Judge::IsAtRoleList()
{
	return rw.ReadInt(游戏状态) == 0;
}

bool Judge::IsAtTown()
{
	return rw.ReadInt(游戏状态) == 1;
	// return false;
}

bool Judge::IsAtMap()
{
	return rw.ReadInt(游戏状态) == 3;
}

bool Judge::IsOpenDoor()
{
	return rw.ReadInt(rw.ReadLong(rw.ReadLong(gd.personPtr + 地图偏移) + 16) + 是否开门) == 0;
}

bool Judge::IsBossRoom()
{
	CoordinateStruct current = GetCurrentRoom();
	CoordinateStruct boss = GetBossRoom();
	return CoordinateEqual(current, boss);
}

bool Judge::CoordinateEqual(CoordinateStruct c1, CoordinateStruct c2)
{
	return c1.x == c2.x && c1.y == c2.y;
}

int Judge::GetRoleLevel()
{
	return rw.ReadInt(角色等级);
}

int Judge::GetRoleNum()
{
	return 	rw.ReadInt(rw.ReadLong(rw.ReadLong(角色基址) + 角色初始指针) + 角色总数偏移);
}

CoordinateStruct Judge::GetCurrentRoom()
{
	CoordinateStruct coordinate = {};
	ULONG64 roomData = rw.ReadLong(rw.ReadLong(rw.ReadLong(房间编号) + 时间基址) + 门型偏移);
	coordinate.x = rw.ReadInt(roomData + 当前房间X);
	coordinate.y = rw.ReadInt(roomData + 当前房间Y);
	return coordinate;
}

CoordinateStruct Judge::GetBossRoom()
{
	CoordinateStruct coordinate = {};
	ULONG64 roomData = rw.ReadLong(rw.ReadLong(rw.ReadLong(房间编号) + 时间基址) + 门型偏移);
	coordinate.x = rw.ReadInt(roomData + BOSS房间X);
	coordinate.y = rw.ReadInt(roomData + BOSS房间Y);
	return coordinate;
}
// todo 
int Judge::GetMapLevelA()
{
	return 0;
}

bool Judge::IsPassMap()
{
	ULONG64 roomData =rw.ReadLong(rw.ReadLong(rw.ReadLong(房间编号) + 时间基址) + 门型偏移);
	INT val = rw.ReadInt(roomData + 篝火判断);
	return val == 2 || val == 0;
}

int Judge::GetFame()
{
	return rw.ReadInt(rw.ReadLong(gd.personAddress) + 人物名望);
}

int Judge::GetPersonAction()
{
	return rw.ReadInt(gd.personPtr + 动作ID);
}

int Judge::GetMapLevel()
{
	gd.mapId = config.ReadConfigItem(configData.mapId);
	gd.mapLevel = config.ReadConfigItem(configData.mapLevel);
	int fame = GetFame();
	int 地图编号 = gd.mapId;
	if (fame > 4176) {
		gd.mapLevel = 0;
	}
	if (fame > 8602) {
		gd.mapLevel = 1;
	}
	if (fame > 13195) {
		gd.mapLevel = 2;
	}
	if (fame > 21675) {
		gd.mapLevel = 3;
	}
	if (fame > 23259) {
		gd.mapLevel = 4;
	}

	if (fame > 25837) {
		gd.mapLevel = 0;
	}
	if (fame > 29369) {
		gd.mapLevel = 1;
	}
	if (fame > 30946) {
		gd.mapLevel = 2;
	}
	if (fame > 32529) {
		gd.mapLevel = 3;
	}
	if (fame > 33989) {
		gd.mapLevel = 4;
	}

	if (fame > 25837)
	{
		if (gd.mapId == 100002964) {
			gd.mapId = 100002974; // 王之摇篮 英豪
		}
		if (gd.mapId == 100002676) {
			gd.mapId = 400001567; // 纳瑟乌森林 英豪
		}
		if (gd.mapId == 100002962) {
			gd.mapId = 100002963; // 柯涅恩山 英豪
		}
	}
	else {
		if (gd.mapId == 100002974) {
			gd.mapId = 100002964; // 王之摇篮 英豪
		}
		if (gd.mapId == 400001567) {
			gd.mapId = 100002676; // 纳瑟乌森林 英豪
		}
		if (gd.mapId == 100002963) {
			gd.mapId = 100002962; // 柯涅恩山 英豪
		}
	}
	return gd.mapLevel;
}

bool Judge::IsDied()
{
	//int maxHp = rw.ReadInt(gd.personPtr + 满血偏移);
	int currentHp = rw.ReadInt(gd.personPtr + 当前血量);
	return GetPersonAction() == 5 || currentHp == 0;
	// return false;
}

bool Judge::IsSkillCoolDown(int position)
{
	ULONG64 skillPtr = gd.personPtr;
	skillPtr = rw.ReadLong(skillPtr + 技能栏);
	skillPtr = rw.ReadLong(skillPtr + 技能栏偏移);
	skillPtr = rw.ReadLong(rw.ReadLong(skillPtr + position * 24) + 16) - 16;
	return GetSkillCoolDownLeftTime(skillPtr) == 0;
}

int Judge::GetSkillCoolDownLeftTime(ULONG64 skill)
{
	ULONG64 coolParam1 = 冷却参数_1;
	ULONG64 coolParam2 = 冷却参数_2 - 8;
	ULONG64 keyParam = 判断冷却_1;
	ULONG64 coolOffset = 判断冷却_2;
	int lastPressTime = rw.ReadInt(skill + keyParam); 
	int eax = rw.ReadInt(skill + coolOffset);  
	int edi = rw.ReadInt(skill + coolOffset + 4); 
	float xmm1 = rw.ReadFloat(skill + coolOffset + 8); 
	int ret = rw.ReadInt(coolParam1 + rw.ReadInt(coolParam2 + 8) * 4) 
		+ rw.ReadInt(coolParam2 + 16); 
	ret = (ret - eax) * (int)xmm1 + edi;
	return ret - lastPressTime > 0 ? 0 : lastPressTime - ret;
}


