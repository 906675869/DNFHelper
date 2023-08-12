#include "action.h"
#include "common.h"
#include "helper.h"
#include "pack.h"
#include "call.h"
#include "keyboard.h"
#include "judge.h"
#include <iostream>
#include "simulate.h"
#include "config.h"

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
		// ULONG loopTypeA = rw.ReadInt(loopPtr + 类型偏移 + 4);
		ULONG camp = rw.ReadInt(loopPtr + 阵营偏移);
		if (loopType == 289)
		{
			ULONG64 goodsPtr = rw.ReadLong(loopPtr + 地面物品);
			wstring	goodName = UnicodeToAnsi(rw.ReadBytes(rw.ReadLong(goodsPtr + 物品名称), 100));
			CoordinateStruct coordinate = cl.ReadCoordinate(loopPtr);
			if (goodName.size() >= 2) {
				cout << "物品坐标(" << coordinate.x << "," << coordinate.y << ")" << endl;
				coordinates.push_back(coordinate);
			}
		}
		if (coordinates.size() == 0) 
		{
			return;
		}
		for (int i = 0; i < coordinates.size(); i++)
		{
			// 漂移至坐标
			// cl.DriftCall(personPtr, coordinates[i].x, coordinates[i].y, 0, 50);
			sl.GoDestation(coordinates[i].x, coordinates[i].y);
			kb.Press(X键);
			Sleep(20);
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
		if (loopType == 529 || loopType == 273 || loopType == 545 || loopType == 61440
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
	int endRandom = jd.IsBossRoom() ? 13 : 10;
	int random = GetRandomNum(-1, endRandom);
	vector<int> keyCodes = { A键 ,S键, D键, F键, G键 ,V键,ALT键,
	  Q键 ,W键, E键, R键, T键 ,Y键,CTRL键 };
	if (random == -1) {
		kb.Press(X键);
		return;
	}
	int keyPosition = random;
	int keycode = keyCodes[random];
	if (jd.IsSkillCoolDown(keyPosition)) {
		// 特殊技能特殊处理
		wstring skillName = jd.GetSkillName(keyPosition);
		kb.Press(keycode);
		if (skillName.find(L"流星") != string::npos) {
			kb.Press(X键);
			// kb.Press(C键);
			kb.Press(Z键);
		}
	}
	if (jd.GetPersonAction() == 0 && random > 3) {
		kb.Press(X键);
		kb.Press(Z键);
	}

}
// 跟随怪物
void Action::FollowMonster()
{	
	if (LoopMonster() == 0) {
		return;
	}
	CoordinateStruct personCoordinate = cl.ReadCoordinate(gd.personPtr);
	ULONG64 monster = gd.globalMonsters[0];
	CoordinateStruct monsterCoordinate = cl.ReadCoordinate(monster);
	int distance = (int)sqrt((pow(abs(personCoordinate.x - monsterCoordinate.x), 2)
		+ pow(abs(personCoordinate.y - monsterCoordinate.y), 2)));
	//  超过一个
	if(gd.globalMonsters.size() > 1){
		for (int i = 0; i < gd.globalMonsters.size(); i++) {
			ULONG64 tmpMonster = gd.globalMonsters[i];
			CoordinateStruct tmpMonsterCoordinate = cl.ReadCoordinate(tmpMonster);
			int tempDistance = (int)sqrt((pow(abs(personCoordinate.x - tmpMonsterCoordinate.x), 2)
				+ pow(abs(personCoordinate.y - tmpMonsterCoordinate.y), 2)));
			if (tempDistance < distance) {
				distance = tempDistance;
				monster = tmpMonster;
				monsterCoordinate = tmpMonsterCoordinate;
			}
		}
	}
	// cout << "计算怪物与人物之间的距离为：" << distance << endl;
	// 判断精度
	if (abs(distance) > 60) {
		// cout << "前往怪物坐标x=" << monsterCoordinate.x << "y="<< monsterCoordinate.y << endl;
		if (config.ReadConfigItem(configData.followModel) == 1) {
			cl.CoordinateCall(monsterCoordinate.x, monsterCoordinate.y, 0);
		}
		if (config.ReadConfigItem(configData.followModel) == 2) {
			cl.DriftCall(gd.personPtr, monsterCoordinate.x, monsterCoordinate.y, 0);
		}
		if (config.ReadConfigItem(configData.followModel) == 3) {
			sl.GoDestation(monsterCoordinate.x, monsterCoordinate.y);
		}
	}else{
		if (monsterCoordinate.x - personCoordinate.x > 0) {
			kb.Press(右光标键);
		}
		else {
			kb.Press(左光标键);
		}

		for (int i = 0; i < 3; i++) {
			RandomSkill();
		}
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



void Action::Invincible()
{
	rw.WriteLong(gd.personPtr + 无敌偏移, 100);
	rw.WriteLong(gd.personPtr + 霸体偏移, 1);
	cout << "霸体无敌开启" << endl;
}

void Action::SuperSpecialBuff()
{
	if (!jd.IsAtMap()) {
		return;
	}
	cl.SpecialEffectCall(1219);
	cl.SpecialEffectCall(1211);
	cl.SpecialEffectCall(165);
	cl.SpecialEffectCall(174);
	cout << "特效buff开启" << endl;
}

void Action::GoodsBuff()
{
	if (!jd.IsAtMap()) {
		return;
	}
	cl.GoodsCall(490013664); // 雪人药 14000力智
	cl.GoodsCall(490014049); // 成长药水14000力智
	cl.GoodsCall(490028988); // 普洱茶60秒 12%双攻
	cout << "物品buff开启" << endl;
}

void Action::SuperScore()
{
	rw.WriteInt(rw.ReadLong(评分基址) + 评分偏移, GetRandomNum(5201314, 9000000));
	cout << "超级评分开启" << endl;
}
