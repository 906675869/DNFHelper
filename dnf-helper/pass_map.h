#pragma once

class PassMap {
public:
	// 组包过图
	static void PackPassMap(int direction);
	// 组包过图上
	static void PackPassMapUp();
	// 组包过图左
	static void PackPassMapLeft();
	// 组包过图下
	static void PackPassMapDown();
	// 组包过图右
	static void PackPassMapRight();

	// 过图处理
	static void PassMapProcess(int direction);

	// 自动寻路
	static void AutoFindRoute();

	// 固定寻路
	static void FixFindRoute();
	// 模拟过图
	static void SimulateOverMap(int direction);

};

extern PassMap pm;