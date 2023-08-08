#pragma once
#include "common.h"

class Judge {
public:
	// 取剩余疲劳
	int GetFatigue();
	// 是否是在选角界面
	bool IsAtRoleList();
	// 是否在城镇
	bool IsAtTown();
	// 是否在地图内
	bool IsAtMap();

	// 是否开门
	bool IsOpenDoor();
	// 是否在boss房间
	bool IsBossRoom();
	// 坐标是否相等
	bool CoordinateEqual(CoordinateStruct c1, CoordinateStruct c2);
	// 获取角色等级
	int GetRoleLevel();
	// 获取角色数量
	int GetRoleNum();

	// 判断_取当前房间
	CoordinateStruct GetCurrentRoom();
	// 取boss房间
	CoordinateStruct GetBossRoom();
	// 获取地图难度
	int GetMapLevelA();
	// 是否通关
	bool IsPassMap();
	// 取角色名望
	int GetFame();

	// 获取人物动作 // 0站立、6跳跃、8普攻、14跑动
	int GetPersonAction();
	// 获取地图等级
	int GetMapLevel();
	// 角色是否死亡
	bool IsDied();
};

extern Judge jd;