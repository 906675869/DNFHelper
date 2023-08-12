#pragma once
#include <windows.h>
#include <vector>
using namespace std;

#define �ո�� 32
#define Esc�� 27
#define ���˽� 192
#define X�� 88
#define F12�� 123

#define �ҹ��� 39
#define ����� 37
#define �¹��� 40
#define �Ϲ��� 38

#define A�� 65
#define S�� 83
#define D�� 68
#define F�� 70
#define G�� 71
#define H�� 72


#define Q�� 81
#define W�� 87
#define E�� 69
#define R�� 82
#define T�� 84
#define V�� 86
#define Y�� 89
#define CTRL�� 17
#define ALT�� 18
#define C�� 67
#define X�� 88
#define Z�� 90
#define F10�� 121
#define F12�� 123

#define �� 2
#define �� 3
#define �� 0
#define �� 1






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
	// �ж��Ƿ�һ��
	BOOL Equals(HotkeyStruct k1, HotkeyStruct k2);
	// �жϼ��Ƿ���
	BOOL KeyPressed(UINT vk);
	// ����ȼ��Ƿ���
	static void CALLBACK CheckHotkeyCallback(HWND hwnd, UINT messgae, UINT idTimeer, DWORD dwtime);

	// ����ȼ�
	ULONG64 AddHotkey(HotkeyStruct hotkey);
};


extern HotKey hk;


class Keyboard {
public:
	// status �ɿ� ����+�ſ�  1 #����_����  3 #����_����  4 #����_�ſ�  ���״̬���ڵ���5��Ϊ������ſ�֮�����ʱ,�ɽ��ĳЩ����
	void Press(int keycode, int status = 0, bool func = 0);
	// f�������⴦��
	void FnPress(int keycode, int status = 0);
	// �Ƿ��ܰ���code
	bool IsFuncCode(int keycode);
	// ��ȡaccii
	int GetACIICode(int keycode);
	// �Ƿ���
	bool IsPressed(int keycode);
};


extern Keyboard kb;


