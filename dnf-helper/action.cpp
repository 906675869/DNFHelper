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
	rw.WriteInt(rw.ReadLong(gd.personAddress) + ���㱶��, (float)x);
	printf(">> �����˺�����\n");
}

void Action::HookHarm()
{
	hookSwitch = !hookSwitch;
	if (hookSwitch)
	{
		ULONG64 randomHarm = GetRandomNum(1666666, 1999999);
		originHook = rw.ReadBytes(ȫ�ֻ�ַ, 10);
		rw.WriteBytes(ȫ�ֻ�ַ, AppendBytes({ 72, 190 }, IntToBytes(randomHarm, 8)));
		printf(">> HOOK�˺�����\n");
	}
	else
	{
		if (sizeof(originHook) > 0)
		{
			rw.WriteBytes(ȫ�ֻ�ַ, originHook);
		}
		printf(">> HOOK�˺��ر�\n");
	}
}

void Action::PackPickUp()
{
	if (rw. ReadInt(��Ϸ״̬) != 3)
	{
		return;
	}
	ULONG64 personPtr = gd.personPtr;
	ULONG64 mapPtr = rw.ReadLong(rw.ReadLong(personPtr + ��ͼƫ��) + 16);
	ULONG64 startAddress = rw.ReadLong(mapPtr + ��ͼ��ʼ2);
	ULONG64 endAddress = rw.ReadLong(mapPtr + ��ͼ����2);
	ULONG loopCnt = (ULONG)(endAddress - startAddress) / 24;
	for (ULONG i = 0; i < loopCnt; i++)
	{
		ULONG64 loopPtr = rw.ReadLong(startAddress + i * 24);
		loopPtr = rw.ReadLong(loopPtr + 16) - 32;
		if (loopPtr == 0 || loopPtr == personPtr) {
			continue;
		}
		ULONG loopType = rw.ReadInt(loopPtr + ����ƫ��);
		ULONG loopTypeA = rw.ReadInt(loopPtr + ����ƫ�� + 4);
		ULONG camp = rw.ReadInt(loopPtr + ��Ӫƫ��);
		// ULONG loopCode = ReadInt(loopPtr + ����ƫ��);
		if (camp == 200 && (loopType == 289 || loopTypeA == 289))
		{
			// ��Ʒ���� = ��Ʒ���� + 1
			// CoordinateStruct ��Ʒ���� = ReadCoordinate(loopPtr);
			ULONG64 goodsPtr = rw.ReadLong(loopPtr + ������Ʒ);
			wstring	goodName = UnicodeToAnsi(rw.ReadBytes(rw.ReadLong(goodsPtr + ��Ʒ����), 100));
			if (sizeof(goodName) >= 2) {
				ULONG ��ƷID = rw. ReadInt(loopPtr + ����ʰȡ);
				if (��ƷID > 0) {
					//goodsList.push_back(��ƷID);
					// goodsNameList.push_back(goodName);
					pk.PackPickup(��ƷID);
					printf("ִ��ʰȡ:%s" , goodName);
				}
			}
		}
	}
}

void Action::CoordinatePickUp()
{
	if (rw.ReadInt(��Ϸ״̬) != 3)
	{
		return;
	}
	ULONG64 personPtr = gd.personPtr;
	ULONG64 mapPtr = rw.ReadLong(rw.ReadLong(personPtr + ��ͼƫ��) + 16);
	ULONG64 startAddress = rw.ReadLong(mapPtr + ��ͼ��ʼ2);
	ULONG64 endAddress = rw.ReadLong(mapPtr + ��ͼ����2);
	ULONG loopCnt = (ULONG)(endAddress - startAddress) / 24;
	vector<CoordinateStruct> coordinates;
	for (ULONG i = 0; i < loopCnt; i++)
	{
		ULONG64 loopPtr = rw.ReadLong(startAddress + i * 24);
		loopPtr = rw.ReadLong(loopPtr + 16) - 32;
		if (loopPtr == 0 || loopPtr == personPtr) {
			continue;
		}
		ULONG loopType = rw.ReadInt(loopPtr + ����ƫ��);
		ULONG loopTypeA = rw.ReadInt(loopPtr + ����ƫ�� + 4);
		ULONG camp = rw.ReadInt(loopPtr + ��Ӫƫ��);
		if (camp == 200 && (loopType == 289 || loopTypeA == 289))
		{
			ULONG64 goodsPtr = rw.ReadLong(loopPtr + ������Ʒ);
			wstring	goodName = UnicodeToAnsi(rw.ReadBytes(rw.ReadLong(goodsPtr + ��Ʒ����), 100));
			if (sizeof(goodName) >= 2) {
				ULONG ��ƷID = rw.ReadInt(loopPtr + ����ʰȡ);
				if (��ƷID > 0) {
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
			// Ư��������
			cl.DriftCall(personPtr, coordinates[i].x, coordinates[i].y, 0, 50);
			kb.Press(X��);
			Sleep(200);
		}

	}


}

int Action::LoopMonster()
{
	if (rw. ReadInt(��Ϸ״̬) != 3)
	{
		return 0;
	}
	//cout << "ִ�б�������" << endl;
	gd.globalMonsters.clear();
	// �������� ȫ��;
	ULONG64 personPtr = gd .personPtr;
	ULONG64 mapPtr = rw.ReadLong(rw.ReadLong(personPtr + ��ͼƫ��) + 16);
	ULONG64 startAddress = rw.ReadLong(mapPtr + ��ͼ��ʼ2);
	ULONG64 endAddress = rw.ReadLong(mapPtr + ��ͼ����2);
	ULONG loopCnt = (ULONG)((endAddress - startAddress) / 24);
	for (ULONG i = 0; i < loopCnt; i++)
	{
		ULONG64 loopPtr = rw.ReadLong(startAddress + i * 24);
		loopPtr = rw.ReadLong(loopPtr + 16) - 32;
		if (loopPtr == 0) {
			continue;
		}
		ULONG loopCamp = rw.ReadInt(loopPtr + ��Ӫƫ��);
		ULONG loopType = rw.ReadInt(loopPtr + ����ƫ��);
		ULONG loopCode = rw.ReadInt(loopPtr + ����ƫ��);
		wstring loopName = UnicodeToAnsi(rw.ReadBytes(rw.ReadLong(loopPtr + ����ƫ��), 100));
		ULONG64 blood = rw.ReadLong(loopPtr + ����Ѫ��);
		if (loopType == 529 || loopType == 273
			// || loopType == 545 || loopType == 61440
			)
		{
			// ��������
			if (loopPtr == personPtr) {
				continue;
			}
			if (loopCamp != 0 && blood != 0 && loopCode != 0)
			{

				// cout << L"����������" << endl;
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
		keycode = A��;
		keyPosition = 0;
	}
	if (random == 1) {
		keycode = S��;
		keyPosition = 1;
	}
	if (random == 2) {
		keycode = D��;
		keyPosition = 2;
	}
	if (random == 3) {
		keycode = F��;
		keyPosition = 3;
	}
	if (random == 4) {
		keycode = G��;
		keyPosition = 4;
	}
	if (random == 5) {
		keycode = Q��;
		keyPosition = 7;
	}
	if (random == 6) {
		keycode = W��;
		keyPosition = 8;
	}
	if (random == 7) {
		keycode = E��;
		keyPosition = 9;
	}
	if (random == 8) {
		keycode = R��;
		keyPosition = 10;
	}
	if (random == 9) {
		keycode = T��;
		keyPosition = 11;
	}
	if (random == 10) {
		keycode = �ҹ���;
		// keyPosition = �ҹ���;
		kb.Press(keycode);

	}

	if (random == 11) {
		keycode = Y��;
		keyPosition = 12;
	}

	if (random == 12) {
		keycode = CTRL��;
		keyPosition = 13;
	}
	if (random == 13) {
		keycode = ALT��;
		keyPosition = 6;
	}
	if (random == 14) {
		keycode = V��;
		keyPosition = 5;
	}

	if (random != 10) {
		//if (IsSkillCoolDown(keyPosition)) {
			kb.Press(keycode);
		//}
	}
	if (jd.GetPersonAction() == 0 && random > 3) {
		kb.Press(X��);
	}
	// ProcessMsg();

}
// �������
void Action::FollowMonster()
{	
	if (LoopMonster() == 0) {
		return;
	}
	ULONG64 monster = gd.globalMonsters[0];
	CoordinateStruct monsterCoordinate = cl.ReadCoordinate(monster);
	// Ư��Call(����, ��������.x, ��������.z, 0);
	CoordinateStruct personCoordinate = cl.ReadCoordinate(gd.personPtr);
	// �жϾ���
	int distance = abs(monsterCoordinate.x - personCoordinate.x);
	if (distance > 80) {
		//cout << "ִ��Ư��" << endl;
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
		rw.WriteInt(gd.personPtr + ������͸, 0);
		rw.WriteInt(gd.personPtr + ��ͼ��͸, 0);
		cout << "����������͸" << endl;
	}
	else 
	{
		rw.WriteInt(gd.personPtr + ������͸, 40);
		rw.WriteInt(gd.personPtr + ��ͼ��͸, 10);
		cout << "�رս�����͸" << endl;
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
