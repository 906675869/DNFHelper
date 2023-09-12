#include "pch.h"
#include "helper.h"
#include <stdio.h>
#include <tlhelp32.h> 

using namespace game;

BOOL game::FindProcessPid(LPCWSTR ProcessName, DWORD& dwPid)
{
	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;
	// Take a snapshot of all processes in the system.
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return(FALSE);
	}

	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hProcessSnap, &pe32))
	{
		CloseHandle(hProcessSnap);          // clean the snapshot object
		return(FALSE);
	}

	BOOL bRet = FALSE;
	do
	{

		if (!lstrcmp(ProcessName, pe32.szExeFile))
		{
			dwPid = pe32.th32ProcessID;
			bRet = TRUE;
			break;
		}

	} while (Process32Next(hProcessSnap, &pe32));
	CloseHandle(hProcessSnap);
	return bRet;
}

CString gLogStr;


void game::CmfcLog(CString logstr)
{
	if (!logEdit) {
		MessageBox(NULL, L"日志组件未准备", L"提示", MB_OK);
		return;
	}
	gLogStr += logstr;
	gLogStr += L"\r\n";
	logEdit->SetWindowTextW(gLogStr);
	logEdit->LineScroll(logEdit->GetLineCount());
}
