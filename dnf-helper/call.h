#pragma once
#include "common.h"

class Call {
public:
	 static vector<BYTE> SubResp(int i);

	 static vector<BYTE> AddResp(int i);

	 static vector<byte> SimpleCall(ULONG64 addr);
	// 内存汇编
	 static void MemoryCompileCall(vector<BYTE> vals);
	 // call
	 // static vector<BYTE> CallPerson(ULONG64 address);

	// 人物call
	 static ULONG64 PersonCall();

	// 技能Call
	 static void SkillCall(ULONG64 人物地址, int 代码, int 伤害, int X, int Y, int Z, float skillSize = NULL);

	// 特效Call
	 static void SpecialEffectCall(int type);
	// 取坐标Call
	 static int GetCoordinateCall(ULONG64 ptr, int direction);
	// 读坐标
	 static CoordinateStruct ReadCoordinate(ULONG64 ptr);
	// 漂移Call
	 static void DriftCall(ULONG ptr, int X, int Y, int Z, int speed = NULL);
	
	// 进入角色Call
	// static void EnterRoleCall(int rolePosition);
	// 选择角色Call
	 //static void ChooseRoleCall(int rolePosition);
	// 退出角色Call
	// static void ExitRoleCall();
	// 放弃Call
	 static void GiveupMissionCall(int taskId);
	 // 跳过任务call
	 static void SkipMissionCall();

	// 提交Call
	 static void SubmitMissionCall(int taskId);
	// 接受Call
	 static void AcceptMissionCall(int taskId);
	// 完成Call
	 static void CompleteMissionCall(int taskId);
	
	// 区域Call
	 static void AreaCall(int mapId);
	// 取地图编号
	 static INT GetMapId();
	// 取地图名称
	 static wstring GetMapName();
	// 物品buff
	 static void GoodsCall(INT goodsCode);
	 // 过图
	 static void OverMapCall(int direction);
	 // 坐标call
	 static void CoordinateCall(int x, int y, int z);

};

extern Call cl;
