#pragma once
#include <windows.h>
#include <afxstr.h>
#include <afxcmn.h>

namespace game{
	// 通过进程名查进程id
	BOOL FindProcessPid(LPCWSTR ProcessName, DWORD& dwPid);

	// 控制台日志
	void CmfcLog(CString logstr);


	extern CEdit* logEdit;
}