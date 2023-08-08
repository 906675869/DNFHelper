#include "keyboard.h"

// ����״̬
int CachedKeyStates[256];
// �����ȼ�
vector<HotkeyStruct> hotkeys;

HotKey hk;
Keyboard kb;

void HotKey::CheckHotkeyCallback(HWND hwnd, UINT messgae, UINT idTimeer, DWORD dwtime)
{
	for (int i = 0; i < hotkeys.size(); i++) {
		HotkeyStruct ht = hotkeys[i];
		if (hk.KeyPressed(ht.normalKey) && hk.KeyPressed(ht.actionKey) && hk.KeyPressed(ht.extraKey)) {
			// todo something
			HANDLE handle = CreateThread(0, 0, ht.callback, (LPVOID)ht.flag, 0, 0);
			if(handle) CloseHandle(handle);
		}
	}
}

BOOL HotKey::Equals(HotkeyStruct k1, HotkeyStruct k2)
{
	return k1.normalKey == k2.normalKey && k1.actionKey == k2.actionKey && k1.extraKey == k2.extraKey;
}

BOOL HotKey::KeyPressed(UINT vk)
{
	if (vk <= 0) {
		return TRUE;
	}
	BOOL flag = FALSE;
	// ��ǰ��������
	if (GetAsyncKeyState(vk) & 0x8000) {
		// ֮ǰ��״̬�ǵ���
		if (CachedKeyStates[vk] == 0) {
			CachedKeyStates[vk] = 1;
			flag = TRUE;
		}
		else {
			CachedKeyStates[vk] = 1;
			flag = FALSE;
		}

	}
	else {
		CachedKeyStates[vk] = 0;
		flag = FALSE;
	}
	return flag;
}

ULONG64 HotKey::AddHotkey(HotkeyStruct hotkey)
{
	if (hotkeys.size() > 256) {
		return 0;
	}
	// �ж��Ƿ��Ѿ����ù���Ӧ���ȼ�
	for (int i = 0; i < hotkeys.size(); i++) {
		HotkeyStruct ht = hotkeys[i];
		if (Equals(ht, hotkey)) {
			ht.callback = hotkey.callback;
			if (ht.flag != 0) {
				return ht.flag;
			}
			return i + 1000001;
		}
	}
	hotkey.flag = hotkeys.size() + 1000001;
	hotkeys.push_back(hotkey);
	// ���ö�ʱ�����Լ�鰴��״̬
	if (hotkey.flag == 1000001) {
		SetTimer(NULL, 16661, 10, (TIMERPROC)CheckHotkeyCallback);
	}
	return hotkeys.size() + 1000001;
}


// ################### Keyboard ######################

// status �ɿ� ����+�ſ�  1 #����_����  3 #����_����  4 #����_�ſ�  ���״̬���ڵ���5��Ϊ������ſ�֮�����ʱ,�ɽ��ĳЩ����
void Keyboard::Press(int keycode, int status, bool func) {
	if (status == 0) {
		status = 10;
	}
	if (!func) {
		func = IsFuncCode(keycode);
	}
	if (func) {
		switch (status) {
		case 3:
			keybd_event(keycode, MapVirtualKey(keycode, 0), 1, 0);
			break;
		case 4:
			keybd_event(keycode, MapVirtualKey(keycode, 0), 3, 0);
			break;
		default:
			keybd_event(keycode, MapVirtualKey(keycode, 0), 1, 0);
			Sleep(status);
			keybd_event(keycode, MapVirtualKey(keycode, 0), 3, 0);
		}
		return;
	}

	switch (status) {
	case 3:
		keybd_event(keycode, MapVirtualKey(keycode, 0), 0, 0);
		break;
	case 4:
		keybd_event(keycode, MapVirtualKey(keycode, 0), 2, 0);
		break;
	default:
		keybd_event(keycode, MapVirtualKey(keycode, 0), 0, 0);
		Sleep(status);
		keybd_event(keycode, MapVirtualKey(keycode, 0), 2, 0);
	}



}

bool Keyboard::IsFuncCode(int keycode) {
	return GetACIICode(keycode) == 0;
}

int Keyboard::GetACIICode(int keycode) {
	int acii = MapVirtualKeyW(keycode, 2);
	if (acii >= 65 && acii <= 90) {
		acii = acii + 32;
	}
	return acii;
}

bool Keyboard::IsPressed(int keycode)
{
	return GetAsyncKeyState(keycode) & 0x8000;
}
