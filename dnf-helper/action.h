#pragma once
class Action {
public:
	// ����
	static void MutiplyHarm(int x = 0);
	// hook�˺�
	static void HookHarm();
	// ����ʰȡ
	static void PackPickUp();
	// ����ʰȡ
	static void CoordinatePickUp();
	// ��������
	static int LoopMonster();
	// �������
	static void RandomSkill();
	// �������
	static void FollowMonster();
	// ���ӽ���
	static void IgnoreBuildings(bool ok);
	// �¼�����
	static void ProcessMsg();
};

extern Action at;