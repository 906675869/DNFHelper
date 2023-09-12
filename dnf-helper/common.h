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
	CoordinateStruct currentCoordinate; // ��ǰ����
	CoordinateStruct bossCoordinate; // boss ����
	wstring mapName; // ��ͼ����

	UINT_PTR Tid;// 
	bool skillSwitch;	// ���ܿ���
	bool hookSwitch; // ���ӿ���
	bool autoSwitch; // �Զ�����
	HANDLE threadHandle; // �߳̾��
	bool couldPassMap; // ����˳ͼ
	int taskId; // ������
	int passMapNum; // ˳ͼ�ƴ�
	int pickupNum; // ʰȡ����
	int mapId; // �������
	int mapLevel;// �����Ѷ�

	int currentRolePosition; // ��ǰλ��
	int totalRoleNum; // ��ɫ����
	bool roleChange; // ��ɫ����

	int pass1 = NULL; // ����1
	int pass2 = NULL; // ����2
	int pass3 = NULL; // ����3
	int repeat = NULL; // �ظ�

	int passTo; //  ˳ͼ����

	int postNum; // ���Ӵ���

	bool getDownNextRoom; // ������ķ���
	bool lastPass; // ���˳ͼ
	//bool firstGoMap = TRUE; // �״ν�ͼ

	ULONG64 personAddress; // �����ַ
	ULONG64 personPtr; // ����ָ��

	vector<ULONG64> globalMonsters;

	bool simulate;
	// ����pid
	int pid;
};

extern GlobalData gd;



struct AutoStruct {
	INT passMapNum;
	// ͨ�ػس� �ӳٴ���
	INT passMapLoopCnt;
	BOOL isPassMapFlag;
	BOOL isOverMap;
	int overMapCnt;
	BOOL isFirstMap;
	BOOL waitChangeRole;
	int townWaitCnt;
	int chooseRoleNum;
	BOOL EnterMap; // ���Խ�ͼ
	BOOL EnableOverRoom; // ���Թ�����

	int pass1;
	int pass2;

	// ģ��ǰ����ַ
	int simulateX;
	int simulateY;

	int pickUpCnt;
	// ���⴦��
	bool specialProcess;

};

extern AutoStruct as;



struct ConfigData
{
	wstring appName; // ��������

	wstring skillCode; // ȫ��code
	wstring skillHarm; // ȫ���˺�
	wstring skillRate; // ȫ��Ƶ��
	wstring skillNum; // ȫ������
	wstring skillSize; // ȫ����С

	wstring autoModel; // �Զ�ģʽ 
	wstring mapId; // ����id
	wstring mapLevel;// �����Ѷ�

	wstring leftFatigue;// Ԥ��ƣ��
	wstring roleNum; // ˢͼ��ɫ
	wstring followModel;// ����ģʽ
	wstring pickupType;// ����ģʽ
	wstring passMapType;// ��ͼģʽ

	wstring goodsBuff; // ��Ʒbuff
	wstring superBuff; // ����buff
	wstring inviciable; // �����޵�
	wstring goldPerson; // С����
	wstring whiteEquip;
	wstring blueEquip;
	wstring purpleEquip;
	wstring pinkEquip;
	wstring SSEquip;
	wstring braveEquip;
	wstring CsEquip;

	wstring activityRole; // ���ɫ

	wstring δ��ȫͼ;

};

extern ConfigData configData;


vector<byte> operator+(vector<byte> a, vector<byte> b);


string BytesToHexStr(vector<byte> bytes);


vector<byte> HexToBytes(const string hex);