#pragma once
class Auto {

public:
	// 自动开关
	static void AutoSwitch();
	// 城镇处理
	static void TownProcess();
	// 每图循环
	static void EveryRoomLoop();
	// 过图处理
	static void OverMap();
	// 通关处理
	static void ClearMap();

};

extern Auto aut;
