#pragma once
#include <windows.h>
#include <vector>
using namespace std;

#define 空格键 32
#define Esc键 27
#define 波浪建 192
#define X键 88
#define F12键 123

#define 右光标键 39
#define 左光标键 37
#define 下光标键 40
#define 上光标键 38

#define A键 65
#define S键 83
#define D键 68
#define F键 70
#define G键 71
#define H键 72


#define Q键 81
#define W键 87
#define E键 69
#define R键 82
#define T键 84
#define V键 86
#define Y键 89
#define CTRL键 17
#define ALT键 18
#define C键 67
#define X键 88
#define Z键 90
#define F10键 121
#define F12键 123

#define 上 2
#define 下 3
#define 左 0
#define 右 1






struct HotkeyStruct
{
	UINT normalKey;
	UINT actionKey;
	UINT extraKey;
	LPTHREAD_START_ROUTINE callback;
	ULONG64 flag = 0;
};

class HotKey {
public:

	// HotKey() {}
	// 判断是否一致
	BOOL Equals(HotkeyStruct k1, HotkeyStruct k2);
	// 判断键是否按下
	BOOL KeyPressed(UINT vk);
	// 检测热键是否按下
	static void CALLBACK CheckHotkeyCallback(HWND hwnd, UINT messgae, UINT idTimeer, DWORD dwtime);

	// 添加热键
	ULONG64 AddHotkey(HotkeyStruct hotkey);
};


extern HotKey hk;


class Keyboard {
public:
	// status 可空 按下+放开  1 #按键_按键  3 #按键_按下  4 #按键_放开  如果状态大于等于5则为按下与放开之间的延时,可解决某些屏蔽
	void Press(int keycode, int status = 0, bool func = 0);
	// f按键特殊处理
	void FnPress(int keycode, int status = 0);
	// 是否功能案件code
	bool IsFuncCode(int keycode);
	// 获取accii
	int GetACIICode(int keycode);
	// 是否按下
	bool IsPressed(int keycode);
};


extern Keyboard kb;


