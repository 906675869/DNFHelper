#pragma once
#include "common.h"

class Judge {
public:
	// ȡʣ��ƣ��
	int GetFatigue();
	// �Ƿ�����ѡ�ǽ���
	bool IsAtRoleList();
	// �Ƿ��ڳ���
	bool IsAtTown();
	// �Ƿ��ڵ�ͼ��
	bool IsAtMap();

	// �Ƿ���
	bool IsOpenDoor();
	// �Ƿ���boss����
	bool IsBossRoom();
	// �����Ƿ����
	bool CoordinateEqual(CoordinateStruct c1, CoordinateStruct c2);
	// ��ȡ��ɫ�ȼ�
	int GetRoleLevel();
	// ��ȡ��ɫ����
	int GetRoleNum();

	// �ж�_ȡ��ǰ����
	CoordinateStruct GetCurrentRoom();
	// ȡboss����
	CoordinateStruct GetBossRoom();
	// ��ȡ��ͼ�Ѷ�
	int GetMapLevelA();
	// �Ƿ�ͨ��
	bool IsPassMap();
	// ȡ��ɫ����
	int GetFame();

	// ��ȡ���ﶯ�� // 0վ����6��Ծ��8�չ���14�ܶ�
	int GetPersonAction();
	// ��ȡ��ͼ�ȼ�
	int GetMapLevel();
	// ��ɫ�Ƿ�����
	bool IsDied();
	// �����Ƿ���ȴ
	bool IsSkillCoolDown(int position);
	// ��ȡ������ȴʣ��ʱ��
	int GetSkillCoolDownLeftTime(ULONG64 skill);
	// ���⼼�ܴ���
	wstring GetSkillName(int position);

	// ��ͼ�д�����Ҫʰȡ����Ʒ
	bool HasGoods();

	// ��ȡ����
	int GetWeight();
};

extern Judge jd;