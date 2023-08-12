#include "windows.h"
#include "common.h"
#include <iostream>
#include "helper.h"
#include "keyboard.h"
#include "action.h"
#include "pass_map.h"
#include "call.h"
#include "simulate.h"
#include "auto.h"
#include "config.h"
#include "fastcall.h"
#include "judge.h"

GlobalData gd;
Simulate sl;
AutoStruct as;
Auto aut;

VOID GetPersonPtr()
{
	bool initFlag = false;
	while (true)
	{
		if (!initFlag && (rw.ReadInt(游戏状态) == 1 || rw.ReadInt(游戏状态) == 3))
		{
			std::cout << "执行获取人物地址...";
			cl.PersonCall();
			gd.personAddress = 全局空白 + 4000;
			gd.personPtr = cl.PersonCall();
			if (gd.personPtr > 0)
			{
				initFlag = true;
				std::cout <<"获取人物基地址和人物指针成功" << gd.personAddress <<":"<< gd.personPtr<<endl ;
			}
			else 
			{
				std::cout << "获取人物基地址和人物指针失败" << endl;
			}

		}
		if (rw.ReadInt(游戏状态) == 0 || rw.ReadInt(游戏状态) == 5)
		{
			initFlag = false;
		}
		if (jd.IsAtMap() && jd.IsDied())
		{
			Sleep(3000);
			// 消耗复活币
			kb.Press(X键);
		}
		Sleep(200);
	}
}

//void testPress() {
//	Sleep(3000);
//	kb.Press(Esc键);
//	kb.Press(VK_F1);
//}



VOID HomeActive()
{
	if (gd.helperInit) {
		printf("⊙o⊙ >>  程序已激活成功，无需再次激活 ！\n");
		return;
	}

	gd.helperInit = TRUE;
	DWORD Pid;
	if (FindProcessPid(L"DNF.exe", Pid))
	{
		gd.pid = Pid;
	}
	else
	{
		printf("游戏未启动\n");
		gd.helperInit = !gd.helperInit;
		return;
	}

	//AddHotkey({ VK_F1, 0, 0, (LPTHREAD_START_ROUTINE)FullScreenSwitch });
	hk.AddHotkey({ VK_F2, 0, 0, (LPTHREAD_START_ROUTINE)at.GoodsBuff });
	hk.AddHotkey({ VK_F3, 0, 0, (LPTHREAD_START_ROUTINE)at.SuperSpecialBuff });
	hk.AddHotkey({ VK_F4, 0, 0, (LPTHREAD_START_ROUTINE)at.Invincible });
	hk.AddHotkey({ VK_F5, 0, 0, (LPTHREAD_START_ROUTINE)at.HookHarm });
	hk.AddHotkey({ VK_F6, 0, 0, (LPTHREAD_START_ROUTINE)at.MutiplyHarm });

	hk.AddHotkey({ VK_END, 0, 0, (LPTHREAD_START_ROUTINE)aut.AutoSwitch });

	hk.AddHotkey({ 上光标键, 18, 0, (LPTHREAD_START_ROUTINE)pm.PackPassMapUp});
	hk.AddHotkey({ 下光标键, 18, 0, (LPTHREAD_START_ROUTINE)pm.PackPassMapDown });
	hk.AddHotkey({ 左光标键, 18, 0, (LPTHREAD_START_ROUTINE)pm.PackPassMapLeft });
	hk.AddHotkey({ 右光标键, 18, 0, (LPTHREAD_START_ROUTINE)pm.PackPassMapRight });

	// hk.AddHotkey({ Esc键, 0, 0, (LPTHREAD_START_ROUTINE)testPress });
	printf("\n");
	printf("⊙贪狼⊙ >>  程序激活成功 ！\n");
	//printf("⊙贪狼⊙ F1  开启技能全屏 ！\n");
	printf("⊙贪狼⊙ F2  开启物品BUFF ！\n");
	printf("⊙贪狼⊙ F3  开启超级BUFF ！\n");
	printf("⊙贪狼⊙ F4  开启霸体无敌 ！\n");
	printf("⊙贪狼⊙ F5  开启HOOK高伤！\n");
	printf("⊙贪狼⊙ F6  开启浮点倍攻！\n");
	//GodNotice(L"⊙贪狼⊙ F6  开启小金人！");
	printf("⊙贪狼⊙ END  开启自动刷图 ！\n");
	config.ConfigInit();
	config.WriteConfigFile();
	// fastCall.InitCode();
	CreateUserThead(GetPersonPtr);
	//CreateUserThead(HalfAutoActive);

}



int main()
{
    std::cout << "按HOME键初始化!\n";

	hk.AddHotkey({ VK_HOME, 0, 0, (LPTHREAD_START_ROUTINE)HomeActive });
	MSG msg = {};
	while (GetMessageA(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

    return system("pause");
}

