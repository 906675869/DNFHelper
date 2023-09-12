#pragma once

#include <Windows.h>
#include <vector>

using namespace std;
namespace game{
	class DmReadWrite {
	public :
		LPVOID ApplyMemory(DWORD len);

		DWORD ReadInt(DWORD64 address);

		DWORD64 ReadLong(DWORD64 address);

		CString ReadBytes(DWORD64 address, INT32 len);

		FLOAT ReadFloat(DWORD64 address);

		BOOL WriteInt(DWORD64 address, DWORD data);

		BOOL WriteLong(DWORD64 address, DWORD64 data);

		BOOL WriteFloat(DWORD64 address, FLOAT data);

		BOOL WriteBytes(ULONG64 address, CString bytes);


	};

	extern DmReadWrite rw;
}