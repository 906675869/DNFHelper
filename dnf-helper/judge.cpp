#include "judge.h"
#include "config.h"
#include <iostream>
Judge jd;
// ��ȡ��ɫƣ��
int Judge::GetFatigue()
{
	return rw.ReadInt(���ƣ��) - rw.ReadInt(��ǰƣ��);
}

bool Judge::IsAtRoleList()
{
	return rw.ReadInt(��Ϸ״̬) == 0;
}

bool Judge::IsAtTown()
{
	return rw.ReadInt(��Ϸ״̬) == 1;
	// return false;
}

bool Judge::IsAtMap()
{
	return rw.ReadInt(��Ϸ״̬) == 3;
}

bool Judge::IsOpenDoor()
{
	return rw.ReadInt(rw.ReadLong(rw.ReadLong(gd.personPtr + ��ͼƫ��) + 16) + �Ƿ���) == 0;
}

bool Judge::IsBossRoom()
{
	CoordinateStruct current = GetCurrentRoom();
	CoordinateStruct boss = GetBossRoom();
	return CoordinateEqual(current, boss);
}

bool Judge::CoordinateEqual(CoordinateStruct c1, CoordinateStruct c2)
{
	return c1.x == c2.x && c1.y == c2.y;
}

int Judge::GetRoleLevel()
{
	return rw.ReadInt(��ɫ�ȼ�);
}

int Judge::GetRoleNum()
{
	return 	rw.ReadInt(rw.ReadLong(rw.ReadLong(��ɫ��ַ) + ��ɫ��ʼָ��) + ��ɫ����ƫ��);
}

CoordinateStruct Judge::GetCurrentRoom()
{
	CoordinateStruct coordinate = {};
	ULONG64 roomData = rw.ReadLong(rw.ReadLong(rw.ReadLong(������) + ʱ���ַ) + ����ƫ��);
	coordinate.x = rw.ReadInt(roomData + ��ǰ����X);
	coordinate.y = rw.ReadInt(roomData + ��ǰ����Y);
	return coordinate;
}

CoordinateStruct Judge::GetBossRoom()
{
	CoordinateStruct coordinate = {};
	ULONG64 roomData = rw.ReadLong(rw.ReadLong(rw.ReadLong(������) + ʱ���ַ) + ����ƫ��);
	coordinate.x = rw.ReadInt(roomData + BOSS����X);
	coordinate.y = rw.ReadInt(roomData + BOSS����Y);
	return coordinate;
}
// todo 
int Judge::GetMapLevelA()
{
	return 0;
}

bool Judge::IsPassMap()
{
	ULONG64 roomData =rw.ReadLong(rw.ReadLong(rw.ReadLong(������) + ʱ���ַ) + ����ƫ��);
	INT val = rw.ReadInt(roomData + �����ж�);
	return val == 2 || val == 0;
}

int Judge::GetFame()
{
	return rw.ReadInt(rw.ReadLong(gd.personAddress) + ��������);
}

int Judge::GetPersonAction()
{
	return rw.ReadInt(gd.personPtr + ����ID);
}

int Judge::GetMapLevel()
{
	gd.mapId = config.ReadConfigItem(configData.mapId);
	gd.mapLevel = config.ReadConfigItem(configData.mapLevel);
	int fame = GetFame();
	int ��ͼ��� = gd.mapId;
	if (fame > 4176) {
		gd.mapLevel = 0;
	}
	if (fame > 8602) {
		gd.mapLevel = 1;
	}
	if (fame > 13195) {
		gd.mapLevel = 2;
	}
	if (fame > 21675) {
		gd.mapLevel = 3;
	}
	if (fame > 23259) {
		gd.mapLevel = 4;
	}

	if (fame > 25837) {
		gd.mapLevel = 0;
	}
	if (fame > 29369) {
		gd.mapLevel = 1;
	}
	if (fame > 30946) {
		gd.mapLevel = 2;
	}
	if (fame > 32529) {
		gd.mapLevel = 3;
	}
	if (fame > 33989) {
		gd.mapLevel = 4;
	}

	if (fame > 25837)
	{
		if (gd.mapId == 100002964) {
			gd.mapId = 100002974; // ��֮ҡ�� Ӣ��
		}
		if (gd.mapId == 100002676) {
			gd.mapId = 400001567; // ��ɪ��ɭ�� Ӣ��
		}
		if (gd.mapId == 100002962) {
			gd.mapId = 100002963; // ������ɽ Ӣ��
		}
	}
	else {
		if (gd.mapId == 100002974) {
			gd.mapId = 100002964; // ��֮ҡ�� Ӣ��
		}
		if (gd.mapId == 400001567) {
			gd.mapId = 100002676; // ��ɪ��ɭ�� Ӣ��
		}
		if (gd.mapId == 100002963) {
			gd.mapId = 100002962; // ������ɽ Ӣ��
		}
	}
	return gd.mapLevel;
}

bool Judge::IsDied()
{
	//int maxHp = rw.ReadInt(gd.personPtr + ��Ѫƫ��);
	int currentHp = rw.ReadInt(gd.personPtr + ��ǰѪ��);
	return GetPersonAction() == 5 || currentHp == 0;
	// return false;
}

bool Judge::IsSkillCoolDown(int position)
{
	ULONG64 skillPtr = gd.personPtr;
	skillPtr = rw.ReadLong(skillPtr + ������);
	skillPtr = rw.ReadLong(skillPtr + ������ƫ��);
	skillPtr = rw.ReadLong(rw.ReadLong(skillPtr + position * 24) + 16) - 16;
	return GetSkillCoolDownLeftTime(skillPtr) == 0;
}

int Judge::GetSkillCoolDownLeftTime(ULONG64 skill)
{
	ULONG64 coolParam1 = ��ȴ����_1;
	ULONG64 coolParam2 = ��ȴ����_2 - 8;
	ULONG64 keyParam = �ж���ȴ_1;
	ULONG64 coolOffset = �ж���ȴ_2;
	int lastPressTime = rw.ReadInt(skill + keyParam); 
	int eax = rw.ReadInt(skill + coolOffset);  
	int edi = rw.ReadInt(skill + coolOffset + 4); 
	float xmm1 = rw.ReadFloat(skill + coolOffset + 8); 
	int ret = rw.ReadInt(coolParam1 + rw.ReadInt(coolParam2 + 8) * 4) 
		+ rw.ReadInt(coolParam2 + 16); 
	ret = (ret - eax) * (int)xmm1 + edi;
	return ret - lastPressTime > 0 ? 0 : lastPressTime - ret;
}


