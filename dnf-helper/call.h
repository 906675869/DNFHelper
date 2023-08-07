#pragma once
#include "common.h"

class Call {
public:
	 static vector<BYTE> SubResp(int i);

	 static vector<BYTE> AddResp(int i);

	 static vector<byte> SimpleCall(ULONG64 addr);
	// �ڴ���
	 static void MemoryCompileCall(vector<BYTE> vals);
	 // call
	 // static vector<BYTE> CallPerson(ULONG64 address);

	// ����call
	 static ULONG64 PersonCall();

	// ����Call
	 static void SkillCall(ULONG64 �����ַ, int ����, int �˺�, int X, int Y, int Z, float skillSize = NULL);

	// ��ЧCall
	 static void SpecialEffectCall(int type);
	// ȡ����Call
	 static int GetCoordinateCall(ULONG64 ptr, int direction);
	// ������
	 static CoordinateStruct ReadCoordinate(ULONG64 ptr);
	// Ư��Call
	 static void DriftCall(ULONG ptr, int X, int Y, int Z, int speed = NULL);
	
	// �����ɫCall
	// static void EnterRoleCall(int rolePosition);
	// ѡ���ɫCall
	 //static void ChooseRoleCall(int rolePosition);
	// �˳���ɫCall
	// static void ExitRoleCall();
	// ����Call
	 static void GiveupMissionCall(int taskId);
	 // ��������call
	 static void SkipMissionCall();

	// �ύCall
	 static void SubmitMissionCall(int taskId);
	// ����Call
	 static void AcceptMissionCall(int taskId);
	// ���Call
	 static void CompleteMissionCall(int taskId);
	
	// ����Call
	 static void AreaCall(int mapId);
	// ȡ��ͼ���
	 static INT GetMapId();
	// ȡ��ͼ����
	 static wstring GetMapName();
	// ��Ʒbuff
	 static void GoodsCall(INT goodsCode);
	 // ��ͼ
	 static void OverMapCall(int direction);
	 // ����call
	 static void CoordinateCall(int x, int y, int z);

};

extern Call cl;
