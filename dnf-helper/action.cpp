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

static wstring filterGoodsNames = L"�绯�����#�ƾɵ�Ƥ��#���¼�Ӳ����#�鲼Ƭ#���¼�Ԫ�ؽᾧ#�������Ƭ#���¼���ʯ";

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
		ULONG64 loopPtr = rw.ReadLong(startAddress + (ULONG64)i * 24);
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
			CoordinateStruct coordinate = cl.ReadCoordinate(loopPtr);
			ULONG64 goodsPtr = rw.ReadLong(loopPtr + ������Ʒ);
			wstring	goodsName = UnicodeToAnsi(rw.ReadBytes(rw.ReadLong(goodsPtr + ��Ʒ����), 100));
			if (sizeof(goodsName) >= 2) {
				ULONG ��ƷID = rw. ReadInt(loopPtr + ����ʰȡ);
				if (��ƷID > 0 && coordinate.z > 0 && filterGoodsNames.find(goodsName) == string::npos) {
					//goodsList.push_back(��ƷID);
					// goodsNameList.push_back(goodName);
					pk.PackPickup(��ƷID);
					printf("ִ��ʰȡ:%s" , goodsName);
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
	vector<wstring> goodNames;
	for (ULONG i = 0; i < loopCnt; i++)
	{
		ULONG64 loopPtr = rw.ReadLong(startAddress + (ULONG64)i * 24);
		loopPtr = rw.ReadLong(loopPtr + 16) - 32;
		if (loopPtr == 0 || loopPtr == personPtr) {
			continue;
		}
		ULONG loopType = rw.ReadInt(loopPtr + ����ƫ��);
		// ULONG loopTypeA = rw.ReadInt(loopPtr + ����ƫ�� + 4);
		ULONG camp = rw.ReadInt(loopPtr + ��Ӫƫ��);
		if (loopType == 289)
		{
			ULONG64 goodsPtr = rw.ReadLong(loopPtr + ������Ʒ);
			wstring	goodsName = UnicodeToAnsi(rw.ReadBytes(rw.ReadLong(goodsPtr + ��Ʒ����), 100));
			CoordinateStruct coordinate = cl.ReadCoordinate(loopPtr);
			if (goodsName.size() >= 2 && filterGoodsNames.find(goodsName) == string::npos && coordinate.z > 0) {
				// cout << "��Ʒ����(" << coordinate.x << "," << coordinate.y << ")" << endl;
				coordinates.push_back(coordinate);
				goodNames.push_back(goodsName);
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
		// cl.DriftCall(personPtr, coordinates[i].x, coordinates[i].y, 0, 50);
		sl.GoDestation(coordinates[i].x, coordinates[i].y, jd.GetCurrentRoom(), true);
		Sleep(800);
		if (goodNames[i] != L"���") {
			kb.Press(X��);
		}
		Sleep(20);
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
		ULONG64 loopPtr = rw.ReadLong(startAddress + (ULONG64)i * 24);
		loopPtr = rw.ReadLong(loopPtr + 16) - 32;
		if (loopPtr == 0) {
			continue;
		}
		ULONG loopCamp = rw.ReadInt(loopPtr + ��Ӫƫ��);
		ULONG loopType = rw.ReadInt(loopPtr + ����ƫ��);
		ULONG loopCode = rw.ReadInt(loopPtr + ����ƫ��);
		wstring loopName = UnicodeToAnsi(rw.ReadBytes(rw.ReadLong(loopPtr + ����ƫ��), 100));
		ULONG64 blood = rw.ReadLong(loopPtr + ����Ѫ��);
		// ��ǰ���ڲ��ȶ����ѷ�
		if (loopCode == 407003631) 
		{
			as.specialProcess = true;
		}
		if (loopType == 529 || loopType == 273 || loopType == 545 || loopType == 61440
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

void Action::SpecialSkill(int keycode, wstring skillName)
{
	if (skillName.find(L"����") != string::npos) {
		kb.Press(C��);
		Sleep(550);
		kb.Press(X��);
		Sleep(550);
		kb.Press(X��);
		Sleep(550);
		kb.Press(Z��);
		Sleep(550);
		kb.Press(Z��);
	}

	if (skillName.find(L"����") != string::npos) {
		Sleep(300);
		kb.Press(�����);
		kb.FnPress(keycode);
		Sleep(300);
		kb.Press(�ҹ���);
		kb.FnPress(keycode);
	}
	if (skillName.find(L"�ε�") != string::npos) {
		Sleep(300);
		kb.FnPress(keycode);
	}
	if (skillName.find(L"���") != string::npos) {
		Sleep(300);
		kb.FnPress(keycode);
		Sleep(300);
		kb.FnPress(keycode);
		Sleep(300);
		kb.Press(Z��);
	}

}



void  Action::RandomSkill() {
	int endRandom = jd.IsBossRoom() ? 13 : 10;
	int random = GetRandomNum(-1, endRandom);
	vector<int> keyCodes = { A�� ,S��, D��, F��, G�� ,V��,ALT��,
	  Q�� ,W��, E��, R��, T�� ,Y��,CTRL�� };
	if (random == -1) {
		kb.Press(X��);
		return;
	}
	int keyPosition = random;
	int keycode = keyCodes[random];
	if (jd.IsSkillCoolDown(keyPosition)) {
		// ���⼼�����⴦��
		wstring skillName = jd.GetSkillName(keyPosition);
		kb.FnPress(keycode);
		SpecialSkill(keycode, skillName);
	}
	if (jd.GetPersonAction() == 0 && random > 3) {
		kb.Press(X��);
		//kb.Press(Z��);
	}

}
// �������
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
	//  ����һ��
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
	// cout << "�������������֮��ľ���Ϊ��" << distance << endl;
	// �жϾ���
	if (abs(distance) > 120) {
		// cout << "ǰ����������x=" << monsterCoordinate.x << "y="<< monsterCoordinate.y << endl;
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
			kb.Press(�ҹ���);
		}
		else {
			kb.Press(�����);
		}

		//for (int i = 0; i < 5; i++) {
		RandomSkill();
		//}
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



void Action::Invincible()
{
	rw.WriteLong(gd.personPtr + �޵�ƫ��, 100);
	rw.WriteLong(gd.personPtr + ����ƫ��, 1);
	cout << "�����޵п���" << endl;
}

void Action::SuperSpecialBuff()
{
	if (!jd.IsAtMap()) {
		return;
	}
	// cl.SpecialEffectCall(1219);
	// cl.SpecialEffectCall(1211);
	cl.SpecialEffectCall(165); // - 30 cd
	// cl.SpecialEffectCall(174);
	cout << "��Чbuff����" << endl;
}

void Action::GoodsBuff()
{
	if (!jd.IsAtMap()) {
		return;
	}
	// cl.GoodsCall(490013664); // ѩ��ҩ 14000����
	// cl.GoodsCall(490014049); // �ɳ�ҩˮ14000����
	cl.GoodsCall(490028988); // �ն���60�� 12%˫��
	cl.GoodsCall(10088698); //  -13 cd
	cout << "��Ʒbuff����" << endl;
}

void Action::SuperScore()
{
	rw.WriteInt(rw.ReadLong(���ֻ�ַ) + ����ƫ��, GetRandomNum(101010, 520314));
	cout << "�������ֿ���" << endl;
}

void Action::EquipProcess()
{
	ULONG64 addrStart = rw.ReadLong(rw.ReadLong(������ַ) + ��Ʒ��ƫ��) + 0x48;
	vector<int> wEquips, bEquips, pEquips;
	for (int i = 0; i < 56; i++) {
		ULONG64 equipAddr = rw.ReadLong(rw.ReadLong(addrStart + i * 8) - 72 + 16);
		if (equipAddr == 0) {
			continue;
		}
		wstring name = UnicodeToAnsi(rw.ReadBytes(rw.ReadLong(equipAddr + ��Ʒ����), 100));
		if (name.size() == 0) {
			continue;
		}

		int level = rw.ReadInt(equipAddr + װ��Ʒ��);
		if (level > 10) {
			continue;
		}
		if (level == 0) {
			wEquips.push_back(i + 9);
		}
		if (level == 1) {
			bEquips.push_back(i + 9);
		}
		if (level == 2) {
			pEquips.push_back(i + 9);
		}
	}
	if (wEquips.size()> 0)
	{
		if (config.ReadConfigItem(configData.whiteEquip) == 1) {
			cout << "ִ���������, ��װ��[" << wEquips.size() << "]��;" << endl;
			pk.PackSale(wEquips.size(), wEquips);
		}
		if (config.ReadConfigItem(configData.whiteEquip) == 2) {
			cout << "ִ��ϵͳ�ֽ�, ��װ��[" << wEquips.size() << "]��;" << endl;
			cl.SystemDecompose(wEquips);
		}
		
		Sleep(3000);
	}
	if (bEquips.size() > 0)
	{
		if (config.ReadConfigItem(configData.blueEquip) == 1) {
			cout << "ִ���������, �߼���[" << wEquips.size() << "]��;" << endl;
			pk.PackSale(bEquips.size(), bEquips);
		}
		if (config.ReadConfigItem(configData.blueEquip) == 2) {
			cout << "ִ��ϵͳ�ֽ�, �߼���[" << wEquips.size() << "]��;" << endl;
			cl.SystemDecompose(bEquips);
		}
		Sleep(3000);
	}
	if (pEquips.size() > 0)
	{
		if (config.ReadConfigItem(configData.purpleEquip) == 1) {
			cout << "ִ���������, ϡ�У�[" << wEquips.size() << "]��;" << endl;
			pk.PackSale(bEquips.size(), pEquips);
		}
		if (config.ReadConfigItem(configData.purpleEquip) == 2) {
			cout << "ִ��ϵͳ�ֽ�, ϡ�У�[" << wEquips.size() << "]��;" << endl;
			cl.SystemDecompose(pEquips);
		}
		Sleep(3000);
	}
	
}
