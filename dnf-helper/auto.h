#pragma once
class Auto {

public:
	// �Զ�����
	static void AutoSwitch();
	// ������
	static void TownProcess();
	// ÿͼѭ��
	static void EveryRoomLoop();
	// ��ͼ����
	static void OverMap();
	// �ٴ�ˢͼ
	static void ContinueMap(bool continueMap);
	// ͨ�ش���
	static void ClearMap();
	// �������
	static void EnterTown();

};

extern Auto aut;
