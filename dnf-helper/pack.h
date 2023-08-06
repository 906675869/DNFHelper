#pragma once
#include "common.h"

// 发包能力
class PackFunc {
public:
	// 出售
	void PackSale(int saleSize, vector<int> postions);
	// 分解
	void PackDecompose(int postion);
	// 拾取
	//void PackPickup(ULONG code);
	// 返回角色列表
	void RoleList();
	// 选择角色
	void ChooseRole(int position);
	// 图内再次挑战
	void ContinueChangle();
	// 出图
	void OutMap();
	// 整理背包
	void TidyupBag(int bagtab, int bagtype, int sorttype);
	// 组包移动
	void Move(int worldMapId, int townMapId, int townX, int townY);
	// 翻牌
	void FlopCard(int x, int y);
	// 过图
	void PassRoom(int x, int y);
	// 进图
	void GetinMap(int mapId, int mapLevel, int abyss, int practice);
	// 选图
	void ChooseMap();
	// 组包拾取
	void PackPickup(int code);
	// 组包过图
	void PackOverMap(int x, int y);



	// 接受任务
	void AcceptTask(int taskId);
	// 放弃任务
	void GiveupTask(int taskId);
	// 完成任务
	void FinishTask(int taskId);
	// 提交任务
	void SubmitTask(int taskId);


};

extern PackFunc pk;