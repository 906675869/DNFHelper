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
	// 再次刷图
	static void ContinueMap(bool continueMap);
	// 通关处理
	static void ClearMap();
	// 进入城镇
	static void EnterTown();

};

extern Auto aut;
