#pragma once
#include <windows.h>
#include <afxstr.h>
#include <afxcmn.h>

namespace game{
	// ͨ�������������id
	BOOL FindProcessPid(LPCWSTR ProcessName, DWORD& dwPid);

	// ����̨��־
	void CmfcLog(CString logstr);


	extern CEdit* logEdit;
}