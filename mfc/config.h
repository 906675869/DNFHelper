#pragma once

namespace game 
{
	enum AUTO_MODEL {COINS=0,STORY=1,WY=2};

	struct Config {
		int autoModel; // 自动模式
		bool floatHarm; // 浮点倍攻
		bool hookHarm; // 固定伤害
		bool pureInvincible;// 透明无敌
		bool btInvincible;// 霸体无敌
		bool speed; // 三速微调
		bool firePerson; // 火人跑图
		bool autoPick; // 自动聚物
		bool fullScreenHook;// 全屏钩子
		bool floatCooldown;// 浮点冷却
		bool skillNoCd; // 技能无cd
	};

	extern Config config;

	struct ProcessConfig {
		// 是否已经初始化大漠
		bool dmInit;

		// 游戏pid
		DWORD gamePid;
		// 当前进程pid
		DWORD currentPid;


	};

	extern ProcessConfig processConfig;



	struct CoordinateStruct {
		int x;
		int y;
		int z;
	};
}

