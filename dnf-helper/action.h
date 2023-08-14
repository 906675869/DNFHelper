#pragma once
class Action {
public:
	// 倍攻
	static void MutiplyHarm(int x = 0);
	// hook伤害
	static void HookHarm();
	// 发包拾取
	static void PackPickUp();
	// 坐标拾取
	static void CoordinatePickUp();
	// 遍历怪物
	static int LoopMonster();
	// 特殊技能处理
	static void SpecialSkill(int keycode, wstring skillName);
	// 随机技能
	static void RandomSkill();
	// 跟随怪物
	static void FollowMonster();
	// 无视建筑
	static void IgnoreBuildings(bool ok);
	// 事件处理
	// static void ProcessMsg();
	// 无敌
	static void Invincible();
	// 超级特效buff
	static void SuperSpecialBuff();
	// 物品buff
	static void GoodsBuff();
	// 超级评分
	static void SuperScore();
};

extern Action at;