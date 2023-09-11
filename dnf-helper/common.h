#pragma once
#include <Windows.h>
#include <vector>
#include <string>
#include "address.h"
#include "readwrite.h"

using namespace std;

constexpr int LEFT = 0;
constexpr int RIGHT = 1;
constexpr int UP = 2;
constexpr int DOWN = 3;

struct CoordinateStruct
{
	int x;
	int y;
	int z;

};

struct GlobalData
{
	bool helperInit = false;
	HWND gameHandle = NULL;
	WNDPROC winProcess;
	CoordinateStruct currentCoordinate; // 当前坐标
	CoordinateStruct bossCoordinate; // boss 坐标
	wstring mapName; // 地图名字

	UINT_PTR Tid;// 
	bool skillSwitch;	// 技能开关
	bool hookSwitch; // 钩子开关
	bool autoSwitch; // 自动开关
	HANDLE threadHandle; // 线程句柄
	bool couldPassMap; // 可以顺图
	int taskId; // 任务编号
	int passMapNum; // 顺图计次
	int pickupNum; // 拾取次数
	int mapId; // 副本编号
	int mapLevel;// 副本难度

	int currentRolePosition; // 当前位置
	int totalRoleNum; // 角色总数
	bool roleChange; // 角色更换

	int pass1 = NULL; // 经过1
	int pass2 = NULL; // 经过2
	int pass3 = NULL; // 经过3
	int repeat = NULL; // 重复

	int passTo; //  顺图方向

	int postNum; // 柱子次数

	bool getDownNextRoom; // 到下面的房间
	bool lastPass; // 最近顺图
	//bool firstGoMap = TRUE; // 首次进图

	ULONG64 personAddress; // 人物基址
	ULONG64 personPtr; // 人物指针

	vector<ULONG64> globalMonsters;

	bool simulate;
	// 进程pid
	int pid;
};

extern GlobalData gd;



struct AutoStruct {
	INT passMapNum;
	// 通关回城 延迟处理
	INT passMapLoopCnt;
	BOOL isPassMapFlag;
	BOOL isOverMap;
	int overMapCnt;
	BOOL isFirstMap;
	BOOL waitChangeRole;
	int townWaitCnt;
	int chooseRoleNum;
	BOOL EnterMap; // 可以进图
	BOOL EnableOverRoom; // 可以过房间

	int pass1;
	int pass2;

	// 模拟前往地址
	int simulateX;
	int simulateY;

	int pickUpCnt;
	// 特殊处理
	bool specialProcess;

};

extern AutoStruct as;



struct ConfigData
{
	wstring appName; // 辅助名称

	wstring skillCode; // 全屏code
	wstring skillHarm; // 全屏伤害
	wstring skillRate; // 全屏频率
	wstring skillNum; // 全屏次数
	wstring skillSize; // 全屏大小

	wstring autoModel; // 自动模式 
	wstring mapId; // 副本id
	wstring mapLevel;// 副本难度

	wstring leftFatigue;// 预留疲劳
	wstring roleNum; // 刷图角色
	wstring followModel;// 跟随模式
	wstring pickupType;// 捡物模式
	wstring passMapType;// 过图模式

	wstring goodsBuff; // 物品buff
	wstring superBuff; // 超级buff
	wstring inviciable; // 霸体无敌
	wstring goldPerson; // 小金人
	wstring whiteEquip;
	wstring blueEquip;
	wstring purpleEquip;
	wstring pinkEquip;
	wstring SSEquip;
	wstring braveEquip;
	wstring CsEquip;

	wstring activityRole; // 活动角色

	wstring 未央全图;

};

extern ConfigData configData;


vector<byte> operator+(vector<byte> a, vector<byte> b);


string BytesToHexStr(vector<byte> bytes);


vector<byte> HexToBytes(const string hex);