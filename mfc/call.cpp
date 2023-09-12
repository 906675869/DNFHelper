#include "call.h"
#include "helper.h"
#include "config.h"
#include "address.h"
#include "dmrw.h"

using namespace game;
DmReadWrite rw;

static boolean syncFlag;

vector<BYTE> Call::SubResp(int i) {
	if (i > 127) {
		return AppendBytes({ 72, 129, 236 }, IntToBytes(i, 4));
	}
	return AppendBytes({ 72, 131, 236 }, { (BYTE)i });

}

vector<BYTE> Call::AddResp(int i) {
	if (i > 127) {
		return AppendBytes({ 72, 129, 196 }, IntToBytes(i, 4));
	}
	return AppendBytes({ 72, 131, 196 }, { (BYTE)i });
}

vector<byte> Call::HookJmp(ULONG64 hookAddr, ULONG64 jumpAddr, vector<byte> extra)
{
	vector<byte> jump = makeByteArray({ 233 }) + IntToBytes(jumpAddr - hookAddr - 5, 4);
	if (extra.size() > 0)
	{
		jump = jump + extra;
	}

	return jump;
}

static bool hookMonsterSwitch = false;

static vector<byte> monsterOriginBytes;

void Call::HookMonster()
{
	ULONG64 hookMonsterAddr = game::HOOK变怪 + 3;
	ULONG64 emptyAddr = 全局空白 + 4100;
	hookMonsterSwitch = !hookMonsterSwitch;
	vector<byte> hookData;
	if (hookMonsterSwitch) {
		monsterOriginBytes = rw.ReadBytes(hookMonsterAddr, 7);

		hookData = hookData + makeByteArray({ 0xB9 }) + IntToBytes(61500, 4);
		hookData = hookData + makeByteArray({ 0xBA }) + IntToBytes(120, 4);
		hookData = hookData + monsterOriginBytes;
		hookData = hookData + HookJmp(emptyAddr + monsterOriginBytes.size(), hookMonsterAddr + 7, { 144 });
		rw.WriteBytes(emptyAddr, hookData);
		rw.WriteBytes(hookMonsterAddr, HookJmp(hookMonsterAddr, emptyAddr, { 144,144 }));
		cout << "变怪开启" << endl;
	}
	else {

		rw.WriteBytes(hookMonsterAddr, monsterOriginBytes);
		rw.WriteBytes(emptyAddr, GetEmptyBytes(50));
		cout << "变怪关闭" << endl;
	}



}

vector<byte> Call::SimpleCall(ULONG64 addr)
{
	vector<byte> shellCode = { 255, 21, 2, 0, 0, 0, 235, 8 };
	shellCode = shellCode + IntToBytes(addr);
	return shellCode;
}


void Call::MemoryCompileCall(vector<BYTE> vals)
{
	// HOOK数据和原始数据
	vector<byte>HOOKdata, HOOKraw;

	//异步执行
	bool asyncExecute = false;

	//汇编中转,空白地址,判断地址
	__int64 asmTransfer, emptyAddress, logicAddress, HOOKasm, HOOKret;
	asmTransfer = 全局空白 + 300;
	emptyAddress = 全局空白 + 500;
	logicAddress = emptyAddress - 100;
	if (asyncExecute) {
		return;
	}

	asyncExecute = true;
	HOOKasm = 汇编CALL;
	HOOKasm += 144;
	HOOKret = HOOKasm + 19;
	HOOKdata = rw.ReadBytes(HOOKasm, 19);
	HOOKraw = HOOKdata;
	HOOKdata = HOOKdata + makeByteArray({ 72,184 }) + IntToBytes(logicAddress);
	HOOKdata = HOOKdata + makeByteArray({ 131, 56, 1, 117, 42, 72, 129, 236, 0, 3, 0, 0 });
	HOOKdata = HOOKdata + makeByteArray({ 72,187 }) + IntToBytes(emptyAddress);
	HOOKdata = HOOKdata + makeByteArray({ 255, 211 });
	HOOKdata = HOOKdata + makeByteArray({ 72,184 }) + IntToBytes(logicAddress);
	HOOKdata = HOOKdata + makeByteArray({ 199, 0, 3, 0, 0, 0 });
	HOOKdata = HOOKdata + makeByteArray({ 72, 129, 196, 0, 3, 0, 0 });
	HOOKdata = HOOKdata + makeByteArray({ 255, 37, 0, 0, 0, 0 }) + IntToBytes(HOOKret);

	if (rw.ReadInt(asmTransfer) == 0)
	{
		rw.WriteBytes(asmTransfer, HOOKdata);
	}

	rw.WriteBytes(emptyAddress, vals + makeByteArray({ 195 }));
	rw.WriteBytes(HOOKasm, makeByteArray({ 255, 37, 0, 0, 0, 0 }) + IntToBytes(asmTransfer) + makeByteArray({ 144, 144, 144, 144, 144 }));
	rw.WriteInt(logicAddress, 1);
	while (rw.ReadInt(logicAddress) == 1)
	{
		Sleep(5);
		// handleEvents();
	}
	rw.WriteBytes(HOOKasm, HOOKraw);
	rw.WriteBytes(emptyAddress, GetEmptyBytes((int)vals.size() + 16));
	asyncExecute = false;
}


ULONG64 Call::PersonCall()
{
	ULONG64 emptyAddress = 全局空白 + 4000;
	vector<byte>asmCode;
	asmCode = makeByteArray({ 72, 131, 236,100 });
	asmCode = asmCode + makeByteArray({ 72,184 }) + IntToBytes(人物CALL);
	asmCode = asmCode + makeByteArray({ 255,208 });
	asmCode = asmCode + makeByteArray({ 72,163 }) + IntToBytes(emptyAddress);
	asmCode = asmCode + makeByteArray({ 72,131,196,100 });
	MemoryCompileCall(asmCode);
	return  rw.ReadLong(emptyAddress);
}



void Call::SkillCall(ULONG64 人物地址, int 代码, int 伤害, int X, int Y, int Z, float skillSize)
{
	ULONG64 emptyAddress = 全局空白 + 1200;
	// ULONG64 技能结构[200] = { NULL };
	rw.WriteLong((ULONG64)emptyAddress, 人物地址);
	rw.WriteInt((ULONG64)emptyAddress + 16, 代码);
	rw.WriteInt((ULONG64)emptyAddress + 20, 伤害);
	rw.WriteInt((ULONG64)emptyAddress + 32, X);
	rw.WriteInt((ULONG64)emptyAddress + 36, Y);
	rw.WriteInt((ULONG64)emptyAddress + 40, Z);
	rw.WriteFloat((ULONG64)emptyAddress + 140, skillSize);
	rw.WriteFloat((ULONG64)emptyAddress + 144, 65535);
	rw.WriteFloat((ULONG64)emptyAddress + 148, 65535);

	vector<BYTE> bytes = { 72, 129, 236, 0, 2, 0, 0 };
	bytes = AppendBytes(bytes, AppendBytes({ 72, 185 }, IntToBytes(emptyAddress, 8)));
	bytes = AppendBytes(bytes, AppendBytes({ 72, 184 }, IntToBytes(技能CALL, 8)));
	bytes = AppendBytes(bytes, { 255, 208, 72, 129, 196, 0, 2, 0, 0 });
	MemoryCompileCall(bytes);
}



int Call::GetCoordinateCall(ULONG64 ptr, int direction)
{
	CoordinateStruct coordinate = ReadCoordinate(ptr);
	if (direction == 0) {
		return coordinate.x;
	}
	if (direction == 1) {
		return coordinate.y;
	}
	if (direction == 2) {
		return coordinate.z;
	}
	return 0;
}

CoordinateStruct Call::ReadCoordinate(ULONG64 ptr)
{
	INT 类型 = rw.ReadInt(ptr + 类型偏移);
	CoordinateStruct coordinate = {};
	if (类型 == 273) {
		ULONG64 指针 = rw.ReadLong(ptr + 读取坐标);
		coordinate.x = (int)rw.ReadFloat(指针 + 0);
		coordinate.y = (int)rw.ReadFloat(指针 + 4);
		coordinate.z = (int)rw.ReadFloat(指针 + 8);
		return coordinate;
	}
	ULONG64 指针 = rw.ReadLong(ptr + 对象坐标);
	coordinate.x = (int)rw.ReadFloat(指针 + 32);
	coordinate.y = (int)rw.ReadFloat(指针 + 36);
	coordinate.z = (int)rw.ReadFloat(指针 + 40);
	return coordinate;
}

void Call::DriftCall(ULONG ptr, int X, int Y, int Z, int speed)
{
	speed = speed < 50 ? 50 : speed;
	vector<BYTE> compileCode = { 72, 129, 236, 0, 8, 0, 0 };
	compileCode = compileCode + makeByteArray({ 185, 241, 0, 0, 0 });
	compileCode = compileCode + makeByteArray({ 72, 184 }) + IntToBytes(申请内存, 8);
	compileCode = compileCode + makeByteArray({ 255, 208 });
	compileCode = compileCode + makeByteArray({ 72, 139, 240, 72, 139, 200 });
	compileCode = compileCode + makeByteArray({ 72, 184 }) + IntToBytes(漂移CALL, 8);
	compileCode = compileCode + makeByteArray({ 255, 208 });
	compileCode = compileCode + makeByteArray({ 185 }) + IntToBytes(X, 4);
	compileCode = compileCode + makeByteArray({ 137, 8 });
	compileCode = compileCode + makeByteArray({ 185 }) + IntToBytes(Y, 4);
	compileCode = compileCode + makeByteArray({ 137, 72, 4 });
	compileCode = compileCode + makeByteArray({ 185 }) + IntToBytes(Z, 4);
	compileCode = compileCode + makeByteArray({ 137, 72, 8, 72, 141, 72, 24 });
	compileCode = compileCode + makeByteArray({ 186 }) + IntToBytes(speed, 4);
	compileCode = compileCode + makeByteArray({ 72, 184 }) + IntToBytes(漂移CALL2, 8);
	compileCode = compileCode + makeByteArray({ 255, 208 });
	compileCode = compileCode +
		makeByteArray({ 51, 219, 137, 95, 48, 199, 135, 224, 0, 0, 0, 2, 0, 0, 0, 72, 141, 69, 136, 72, 137,
			68, 36, 96, 72, 137, 93, 136, 72, 137, 93, 144, 51, 210, 72, 141, 77, 136 });
	compileCode = compileCode + makeByteArray({ 72, 184 }) + IntToBytes(写入内存, 8);
	compileCode = compileCode + makeByteArray({ 72, 139, 206, 72, 139, 1 });
	compileCode = compileCode +
		makeByteArray({ 72, 139, 6, 137, 92, 36, 64, 72, 137, 92, 36, 56, 72, 137, 92, 36, 48, 137, 92, 36,
			40, 72, 141, 77, 136, 72, 137, 76, 36, 32, 69, 51, 201 });
	compileCode = compileCode + makeByteArray({ 72, 186 }) + IntToBytes(ptr, 8);
	compileCode = compileCode + makeByteArray({ 73, 184 }) + IntToBytes(ptr, 8);
	compileCode = compileCode + makeByteArray({ 72, 139, 206 });
	compileCode = compileCode + makeByteArray({ 255, 144 }) + IntToBytes(312, 4);
	compileCode = compileCode + makeByteArray({ 72, 129, 196, 0, 8, 0, 0 });
	MemoryCompileCall(compileCode);
}

//VOID CompileCall::RunCall(int x, int y, int z, int speed)
//{
//	if (speed < 50) {
//		speed = 50;
//	}
//	vector<BYTE> compileCode = AppendBytes(SubResp(256), AppendBytes({ 0x48, 0xB8 }, IntToBytes(人物CALL, 8)));
//	compileCode = AppendBytes(compileCode, AppendBytes({ 0xFF, 0xD0, 0x48, 0x8B, 0xF0, 0x4C, 0x8B, 0x06, 0x48, 0x8D, 0x54, 0x24, 0x48, 0x48, 0x8B, 0xCE, 0x41, 0xFF, 0x90 }, IntToBytes(奔跑偏移_1, 4)));
//
//
//	
//	
//}

//AreaStruct CompileCall::GetArea(int mapId)
//{
//	
//	return AreaStruct();
//}

//void Call::EnterRoleCall(int rolePosition)
//{
//}
//
//void Call::ChooseRoleCall(int rolePosition)
//{
//}
//
//void Call::ExitRoleCall()
//{
//}
//
void Call::GiveupMissionCall(int taskId)
{
	printf("GiveupMissionCall -> 未完成");
}

void Call::SkipMissionCall()
{
	vector<byte> shellCode = SubResp(512);
	shellCode = shellCode + makeByteArray({ 65, 131, 201, 255 });
	shellCode = shellCode + makeByteArray({ 69, 9, 200 });
	shellCode = shellCode + makeByteArray({ 186, 1, 0, 0, 0 });
	shellCode = shellCode + makeByteArray({ 72, 185 }) + IntToBytes(任务基址, 8);
	shellCode = shellCode + makeByteArray({ 72, 139, 9 });
	shellCode = shellCode, SimpleCall(跳过CALL);
	shellCode = shellCode + AddResp(512);
	MemoryCompileCall(shellCode);
}

void Call::SubmitMissionCall(int taskId)
{
	vector<byte> shellCode = SubResp(48);
	shellCode = shellCode + makeByteArray({ 65, 189, 1, 0, 0, 0 });
	shellCode = shellCode + makeByteArray({ 65, 190, 255, 255, 255, 255 });
	shellCode = shellCode + makeByteArray({ 69, 139, 205 });
	shellCode = shellCode + makeByteArray({ 69, 139, 198 });
	shellCode = shellCode + makeByteArray({ 72, 185 }) + IntToBytes(任务基址, 8);
	shellCode = shellCode + makeByteArray({ 72, 139, 9 });
	shellCode = shellCode + makeByteArray({ 186 }) + IntToBytes(taskId, 4);
	shellCode = shellCode + SimpleCall(提交CALL);
	shellCode = shellCode + AddResp(48);
	MemoryCompileCall(shellCode);
}

void Call::AcceptMissionCall(int taskId)
{
	vector<byte> shellCode = SubResp(40);
	shellCode = shellCode + makeByteArray({ 186 }) + IntToBytes(taskId, 4);
	shellCode = shellCode + SimpleCall(接受CALL);
	shellCode = shellCode + AddResp(40);
	MemoryCompileCall(shellCode);
}

void Call::CompleteMissionCall(int taskId)
{

	vector<byte> shellCode = SubResp(512);
	shellCode = shellCode + makeByteArray({ 179, 255 });
	shellCode = shellCode + makeByteArray({ 68, 15, 182, 203 });
	shellCode = shellCode + makeByteArray({ 65, 176, 255 });
	shellCode = shellCode + makeByteArray({ 186 }) + IntToBytes(taskId, 4);
	shellCode = shellCode + SimpleCall(完成CALL);
	shellCode = shellCode + AddResp(512);
	MemoryCompileCall(shellCode);
}



void Call::AreaCall(int mapId)
{
	ULONG64 areaAddress = rw.ReadLong(区域参数);
	vector<BYTE> compileCode = { 72, 131, 236, 48 };
	compileCode = compileCode + makeByteArray({ 65, 184 }) + IntToBytes(mapId, 4);
	compileCode = compileCode + makeByteArray({ 186, 174, 12, 0, 0 });
	compileCode = compileCode + makeByteArray({ 72, 184, 255, 255, 255, 255, 0, 0, 0, 0 });
	compileCode = compileCode + makeByteArray({ 72, 185 }) + IntToBytes(区域参数, 8);
	compileCode = compileCode + makeByteArray({ 72, 139, 9 }) + makeByteArray({ 76, 139, 201, 73, 129, 193 });
	compileCode = compileCode + IntToBytes(区域偏移, 4) + makeByteArray({ 73, 131, 233, 64 });
	compileCode = compileCode + makeByteArray({ 72, 184 }) + IntToBytes(区域CALL, 8);
	compileCode = compileCode + makeByteArray({ 255, 208, 72, 131, 196, 48 });
	MemoryCompileCall(compileCode);

	int wordAreaId = rw.ReadInt(areaAddress + 区域偏移);
	int mapAreaId = rw.ReadInt(areaAddress + 区域偏移 + 4);
	int X = rw.ReadInt(areaAddress + 区域偏移 + 8);
	int Y = rw.ReadInt(areaAddress + 区域偏移 + 12);

	pk.Move(wordAreaId, mapAreaId, X, Y);
}

INT Call::GetMapId()
{
	return rw.ReadInt(副本编号); ;
}

wstring Call::GetMapName()
{
	vector<byte> bytes = rw.ReadBytes(rw.ReadLong(rw.ReadLong(rw.ReadLong(rw.ReadLong(房间编号) + 时间基址) + 门型偏移) + 地图名称) + 0, 100);
	return UnicodeToAnsi(bytes);
}

//VOID Call::TownMoveCall(int area, int no, int x, int y)
//{
//	return VOID();
//}

void Call::GoodsCall(INT goodsCode)
{
	ULONG64 personAddress = gd.personAddress;
	if (personAddress == 0) {
		return;
	}
	vector<BYTE> compileData = AppendBytes(SubResp(40), { 72, 184 });
	compileData = AppendBytes(compileData, IntToBytes(personAddress, 8));
	compileData = AppendBytes(compileData, { 72, 139, 0, 76, 139, 0, 186 });
	compileData = AppendBytes(compileData, IntToBytes(goodsCode, 4));
	compileData = AppendBytes(compileData, { 72, 139, 200, 65, 255, 144 });
	compileData = AppendBytes(compileData, IntToBytes(物品CALL, 4));
	compileData = AppendBytes(compileData, AddResp(40));
	MemoryCompileCall(compileData);
}


void Call::OverMapCall(int direction)
{
	if (!jd.IsAtMap()) {
		return;
	}
	if (!jd.IsOpenDoor()) {
		return;
	}
	ULONG64 emptyAddr = (ULONG64)rw.ApplyMemory(2048);
	ULONG64 roomData = rw.ReadLong(rw.ReadLong(rw.ReadLong(房间编号) + 时间基址) + 顺图偏移);
	vector<byte> shellCode = { 65, 185, 255, 255, 255, 255 };
	shellCode = shellCode + makeByteArray({ 73, 184 }) + IntToBytes(emptyAddr, 8);
	shellCode = shellCode + makeByteArray({ 186 }) + IntToBytes(direction, 4);
	shellCode = shellCode + makeByteArray({ 72, 185 }) + IntToBytes(roomData, 8);
	shellCode = shellCode + makeByteArray({ 72, 184 }) + IntToBytes(过图CALL, 8);
	shellCode = shellCode + makeByteArray({ 255, 208 });
	MemoryCompileCall(shellCode);
}

void Call::CoordinateCall(int x, int y, int z)
{
	if (gd.personPtr < 1) {
		return;
	}
	vector<byte> shellCode = { 72, 129, 236, 0, 1, 0, 0 };
	shellCode = shellCode + makeByteArray({ 65, 185 }) + IntToBytes(z, 4);
	shellCode = shellCode + makeByteArray({ 65, 184 }) + IntToBytes(y, 4);
	shellCode = shellCode + makeByteArray({ 186 }) + IntToBytes(x, 4);
	shellCode = shellCode + makeByteArray({ 72, 185 }) + IntToBytes(gd.personPtr);
	shellCode = shellCode + makeByteArray({ 72, 139, 1 });
	shellCode = shellCode + makeByteArray({ 255, 144 }) + IntToBytes(坐标CALL偏移, 4);
	shellCode = shellCode + makeByteArray({ 72, 129, 196, 0, 1, 0, 0 });
	MemoryCompileCall(shellCode);

}

void Call::SystemDecompose(vector<int>  positions)
{
	int decomposeId = 317;
	for (int i = 0; i < positions.size(); i++)
	{
		fastCall.Call(分解添加CALL, { rw.ReadLong(背包基址), (ULONG64)positions[i],(ULONG64)decomposeId });
		Sleep(600);
	}
	fastCall.Call(分解CALL, { rw.ReadLong(背包基址), (ULONG64)65535,(ULONG64)decomposeId });
}

// 生成装备call
void Call::GenerateEquipmentCall(ULONG64 objPtr, ULONG64 equipCode)
{
	vector<BYTE> compileDate = makeByteArray({ 72, 129, 236, 0, 1, 0, 0, 65, 185, 1, 0, 0, 0, 65, 184, 1, 0, 0, 0, 72, 199, 194, 0, 0, 0, 0, 72, 185 });
	compileDate = compileDate + IntToBytes(equipCode, 8);
	compileDate = compileDate + makeByteArray({ 72, 184 });
	compileDate = compileDate + IntToBytes(创建CALL, 8);
	compileDate = compileDate + makeByteArray({ 255, 208, 72, 139, 208, 72, 185 });
	compileDate = compileDate + IntToBytes(objPtr, 8);
	compileDate = compileDate + makeByteArray({ 72, 184 });
	compileDate = compileDate + IntToBytes(生效Call, 8);
	compileDate = compileDate + makeByteArray({ 255, 208, 72, 185 });
	compileDate = compileDate + IntToBytes(objPtr, 8);
	compileDate = compileDate + makeByteArray({ 72, 186 });
	compileDate = compileDate + IntToBytes(1, 8);
	compileDate = compileDate + makeByteArray({ 72, 184 });
	compileDate = compileDate + IntToBytes(穿上CALL, 8);
	compileDate = compileDate + makeByteArray({ 255, 208, 72, 129, 196, 0, 1, 0, 0 });
	MemoryCompileCall(compileDate);
}


void Call::SpecialEffectCall(int code) {
	ULONG64 emptyAddr = 全局空白 + 2200;
	vector<BYTE> compileData = makeByteArray({ 72, 129, 236, 0, 1, 0, 0 }) + makeByteArray({ 72, 186 }) + IntToBytes(1, 8);
	compileData = compileData + makeByteArray({ 72, 185 }) + IntToBytes(emptyAddr + 8, 8);
	compileData = compileData + makeByteArray({ 72, 184 }) + IntToBytes(特效缓冲CALL, 8);
	compileData = compileData + makeByteArray({ 255, 208, 72, 186 }) + LongToBytes((ULONG)2);
	compileData = compileData + makeByteArray({ 72, 185 }) + IntToBytes(emptyAddr + 8, 8);
	compileData = compileData + makeByteArray({ 72, 184 }) + IntToBytes(特效缓冲CALL, 8);
	compileData = compileData + makeByteArray({ 255, 208, 72, 185 }) + IntToBytes(rw.ReadLong(特效基址), 8);
	compileData = compileData + makeByteArray({ 73, 184 }) + IntToBytes(emptyAddr, 8);
	compileData = compileData + makeByteArray({ 72, 199, 194 }) + IntToBytes(code, 4);
	compileData = compileData + makeByteArray({ 72, 184 }) + IntToBytes(BUFF参数1, 8);
	compileData = compileData + makeByteArray({ 255, 208, 72, 137, 199, 72, 184 }) + IntToBytes(特效释放CALL, 8);
	compileData = compileData + makeByteArray({ 255, 208, 72, 137, 193, 72, 137, 250, 72, 184 }) + IntToBytes(BUFF参数2, 8);
	compileData = compileData + makeByteArray({ 255, 208, 72, 184 }) + IntToBytes(特效释放CALL, 8);
	compileData = compileData + makeByteArray({ 255, 208, 72, 129, 196, 0, 1, 0, 0 });
	MemoryCompileCall(compileData);
}