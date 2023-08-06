#include "action.h"
#include "common.h"
#include "helper.h"
#include "pack.h"
#include "call.h"
#include "keyboard.h"
#include "judge.h"
#include <iostream>
#include "simulate.h"

Action at;

bool hookSwitch = false;
vector<BYTE> originHook;


void Action::MutiplyHarm(int x)
{
	if (x == 0)
	{
		x = 20;
	}
	rw.WriteInt(rw.ReadLong(gd.personAddress) + 浮点倍攻, (float)x);
	printf(">> 浮点伤害开启\n");
}

void Action::HookHarm()
{
	hookSwitch = !hookSwitch;
	if (hookSwitch)
	{
		ULONG64 randomHarm = GetRandomNum(1666666, 1999999);
		originHook = rw.ReadBytes(全局基址, 10);
		rw.WriteBytes(全局基址, AppendBytes({ 72, 190 }, IntToBytes(randomHarm, 8)));
		printf(">> HOOK伤害开启\n");
	}
	else
	{
		if (sizeof(originHook) > 0)
		{
			rw.WriteBytes(全局基址, originHook);
		}
		printf(">> HOOK伤害关闭\n");
	}
}

void Action::PackPickUp()
{
	if (rw. ReadInt(游戏状态) != 3)
	{
		return;
	}
	ULONG64 personPtr = gd.personPtr;
	ULONG64 mapPtr = rw.ReadLong(rw.ReadLong(personPtr + 地图偏移) + 16);
	ULONG64 startAddress = rw.ReadLong(mapPtr + 地图开始2);
	ULONG64 endAddress = rw.ReadLong(mapPtr + 地图结束2);
	ULONG loopCnt = (ULONG)(endAddress - startAddress) / 24;
	for (ULONG i = 0; i < loopCnt; i++)
	{
		ULONG64 loopPtr = rw.ReadLong(startAddress + i * 24);
		loopPtr = rw.ReadLong(loopPtr + 16) - 32;
		if (loopPtr == 0 || loopPtr == personPtr) {
			continue;
		}
		ULONG loopType = rw.ReadInt(loopPtr + 类型偏移);
		ULONG loopTypeA = rw.ReadInt(loopPtr + 类型偏移 + 4);
		ULONG camp = rw.ReadInt(loopPtr + 阵营偏移);
		// ULONG loopCode = ReadInt(loopPtr + 代码偏移);
		if (camp == 200 && (loopType == 289 || loopTypeA == 289))
		{
			// 物品数量 = 物品数量 + 1
			// CoordinateStruct 物品坐标 = ReadCoordinate(loopPtr);
			ULONG64 goodsPtr = rw.ReadLong(loopPtr + 地面物品);
			wstring	goodName = UnicodeToAnsi(rw.ReadBytes(rw.ReadLong(goodsPtr + 物品名称), 100));
			if (sizeof(goodName) >= 2) {
				ULONG 物品ID = rw. ReadInt(loopPtr + 发包拾取);
				if (物品ID > 0) {
					//goodsList.push_back(物品ID);
					// goodsNameList.push_back(goodName);
					pk.PackPickup(物品ID);
					printf("执行拾取:%s" , goodName);
				}
			}
		}
	}
}

void Action::CoordinatePickUp()
{
	if (rw.ReadInt(游戏状态) != 3)
	{
		return;
	}
	ULONG64 personPtr = gd.personPtr;
	ULONG64 mapPtr = rw.ReadLong(rw.ReadLong(personPtr + 地图偏移) + 16);
	ULONG64 startAddress = rw.ReadLong(mapPtr + 地图开始2);
	ULONG64 endAddress = rw.ReadLong(mapPtr + 地图结束2);
	ULONG loopCnt = (ULONG)(endAddress - startAddress) / 24;
	vector<CoordinateStruct> coordinates;
	for (ULONG i = 0; i < loopCnt; i++)
	{
		ULONG64 loopPtr = rw.ReadLong(startAddress + i * 24);
		loopPtr = rw.ReadLong(loopPtr + 16) - 32;
		if (loopPtr == 0 || loopPtr == personPtr) {
			continue;
		}
		ULONG loopType = rw.ReadInt(loopPtr + 类型偏移);
		ULONG loopTypeA = rw.ReadInt(loopPtr + 类型偏移 + 4);
		ULONG camp = rw.ReadInt(loopPtr + 阵营偏移);
		if (camp == 200 && (loopType == 289 || loopTypeA == 289))
		{
			ULONG64 goodsPtr = rw.ReadLong(loopPtr + 地面物品);
			wstring	goodName = UnicodeToAnsi(rw.ReadBytes(rw.ReadLong(goodsPtr + 物品名称), 100));
			if (sizeof(goodName) >= 2) {
				ULONG 物品ID = rw.ReadInt(loopPtr + 发包拾取);
				if (物品ID > 0) {
					coordinates.push_back(cl.ReadCoordinate(loopPtr));
				}
			}
		}
		if (coordinates.size() == 0) 
		{
			return;
		}
		for (int i = 0; i < coordinates.size(); i++)
		{
			// 漂移至坐标
			cl.DriftCall(personPtr, coordinates[i].x, coordinates[i].y, 0, 50);
			kb.Press(X键);
			Sleep(200);
		}

	}


}

int Action::LoopMonster()
{
	if (rw. ReadInt(游戏状态) != 3)
	{
		return 0;
	}
	//cout << "执行遍历怪物" << endl;
	gd.globalMonsters.clear();
	// 遍历数据 全屏;
	ULONG64 personPtr = gd .personPtr;
	ULONG64 mapPtr = rw.ReadLong(rw.ReadLong(personPtr + 地图偏移) + 16);
	ULONG64 startAddress = rw.ReadLong(mapPtr + 地图开始2);
	ULONG64 endAddress = rw.ReadLong(mapPtr + 地图结束2);
	ULONG loopCnt = (ULONG)((endAddress - startAddress) / 24);
	for (ULONG i = 0; i < loopCnt; i++)
	{
		ULONG64 loopPtr = rw.ReadLong(startAddress + i * 24);
		loopPtr = rw.ReadLong(loopPtr + 16) - 32;
		if (loopPtr == 0) {
			continue;
		}
		ULONG loopCamp = rw.ReadInt(loopPtr + 阵营偏移);
		ULONG loopType = rw.ReadInt(loopPtr + 类型偏移);
		ULONG loopCode = rw.ReadInt(loopPtr + 代码偏移);
		wstring loopName = UnicodeToAnsi(rw.ReadBytes(rw.ReadLong(loopPtr + 名称偏移), 100));
		ULONG64 blood = rw.ReadLong(loopPtr + 怪物血量);
		if (loopType == 529 || loopType == 273
			// || loopType == 545 || loopType == 61440
			)
		{
			// 过滤人物
			if (loopPtr == personPtr) {
				continue;
			}
			if (loopCamp != 0 && blood != 0 && loopCode != 0)
			{

				// cout << L"遍历到怪物" << endl;
				gd.globalMonsters.push_back(loopPtr);
			}
		}
	}
	return gd.globalMonsters.size();
}



void  Action::RandomSkill() {
	int endRandom = jd.IsBossRoom() ? 14 : 10;
	int random = GetRandomNum(0, endRandom);
	int keycode = 0, keyPosition = 0;

	if (random == 0) {
		keycode = A键;
		keyPosition = 0;
	}
	if (random == 1) {
		keycode = S键;
		keyPosition = 1;
	}
	if (random == 2) {
		keycode = D键;
		keyPosition = 2;
	}
	if (random == 3) {
		keycode = F键;
		keyPosition = 3;
	}
	if (random == 4) {
		keycode = G键;
		keyPosition = 4;
	}
	if (random == 5) {
		keycode = Q键;
		keyPosition = 7;
	}
	if (random == 6) {
		keycode = W键;
		keyPosition = 8;
	}
	if (random == 7) {
		keycode = E键;
		keyPosition = 9;
	}
	if (random == 8) {
		keycode = R键;
		keyPosition = 10;
	}
	if (random == 9) {
		keycode = T键;
		keyPosition = 11;
	}
	if (random == 10) {
		keycode = 右光标键;
		// keyPosition = 右光标键;
		kb.Press(keycode);

	}

	if (random == 11) {
		keycode = Y键;
		keyPosition = 12;
	}

	if (random == 12) {
		keycode = CTRL键;
		keyPosition = 13;
	}
	if (random == 13) {
		keycode = ALT键;
		keyPosition = 6;
	}
	if (random == 14) {
		keycode = V键;
		keyPosition = 5;
	}

	if (random != 10) {
		//if (IsSkillCoolDown(keyPosition)) {
			kb.Press(keycode);
		//}
	}
	if (jd.GetPersonAction() == 0 && random > 3) {
		kb.Press(X键);
	}
	// ProcessMsg();

}
// 跟随怪物
void Action::FollowMonster()
{	
	if (LoopMonster() == 0) {
		return;
	}
	ULONG64 monster = gd.globalMonsters[0];
	CoordinateStruct monsterCoordinate = cl.ReadCoordinate(monster);
	// 漂移Call(怪物, 怪物坐标.x, 怪物坐标.z, 0);
	CoordinateStruct personCoordinate = cl.ReadCoordinate(gd.personPtr);
	// 判断精度
	int distance = abs(monsterCoordinate.x - personCoordinate.x);
	if (distance > 80) {
		//cout << "执行漂移" << endl;
		// cl.DriftCall(gd.personPtr, monsterCoordinate.x, monsterCoordinate.y, 0, abs(monsterCoordinate.x - personCoordinate.x));
		// Sleep(200);
		sl.GoDestation(monsterCoordinate.x, monsterCoordinate.y);
	}
	for (int i = 0; i < 3; i++) {
		RandomSkill();
	}
}

void Action::IgnoreBuildings(bool ok)
{
	if (ok) 
	{
		rw.WriteInt(gd.personPtr + 建筑穿透, 0);
		rw.WriteInt(gd.personPtr + 地图穿透, 0);
		cout << "开启建筑穿透" << endl;
	}
	else 
	{
		rw.WriteInt(gd.personPtr + 建筑穿透, 40);
		rw.WriteInt(gd.personPtr + 地图穿透, 10);
		cout << "关闭建筑穿透" << endl;
	}

}

void Action::ProcessMsg()
{
	MSG msg;
	memset(&msg, 0, sizeof(MSG));
	while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

}
