#pragma once
#include "common.h"

// ��������
class PackFunc {
public:
	// ����
	void PackSale(int saleSize, vector<int> postions);
	// �ֽ�
	void PackDecompose(int postion);
	// ʰȡ
	//void PackPickup(ULONG code);
	// ���ؽ�ɫ�б�
	void RoleList();
	// ѡ���ɫ
	void ChooseRole(int position);
	// ͼ���ٴ���ս
	void ContinueChangle();
	// ��ͼ
	void OutMap();
	// ������
	void TidyupBag(int bagtab, int bagtype, int sorttype);
	// ����ƶ�
	void Move(int worldMapId, int townMapId, int townX, int townY);
	// ����
	void FlopCard(int x, int y);
	// ��ͼ
	void PassRoom(int x, int y);
	// ��ͼ
	void GetinMap(int mapId, int mapLevel, int abyss, int practice);
	// ѡͼ
	void ChooseMap();
	// ���ʰȡ
	void PackPickup(int code);
	// �����ͼ
	void PackOverMap(int x, int y);



	// ��������
	void AcceptTask(int taskId);
	// ��������
	void GiveupTask(int taskId);
	// �������
	void FinishTask(int taskId);
	// �ύ����
	void SubmitTask(int taskId);


};

extern PackFunc pk;