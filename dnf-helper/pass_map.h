#pragma once

class PassMap {
public:
	// �����ͼ
	static void PackPassMap(int direction);
	// �����ͼ��
	static void PackPassMapUp();
	// �����ͼ��
	static void PackPassMapLeft();
	// �����ͼ��
	static void PackPassMapDown();
	// �����ͼ��
	static void PackPassMapRight();

	// ��ͼ����
	static void PassMapProcess(int direction);

	// �Զ�Ѱ·
	static void AutoFindRoute();

	// �̶�Ѱ·
	static void FixFindRoute();
	// ģ���ͼ
	static void SimulateOverMap(int direction);

};

extern PassMap pm;