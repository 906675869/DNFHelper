#include "common.h"
#include "readwrite.h"

ReadWrite rw;

LPVOID ReadWrite::ApplyMemory( DWORD len)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, gd.pid);
	LPVOID address = VirtualAllocEx(hProcess, 0, len, 4096, 64);
	CloseHandle(hProcess);
	return address;
}


// ok
DWORD ReadWrite::ReadInt( DWORD64 address)
{
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, gd.pid);
	if (handle == NULL)
	{
		return -1;
	}

	int value;
	if (ReadProcessMemory(handle, (void*)address, &value, 4, NULL))
	{
		return value;
	}

	CloseHandle(handle);

	return -1;
}
// ok
DWORD64 ReadWrite::ReadLong( DWORD64 address)
{
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, gd.pid);
	if (handle == NULL)
	{
		return -1;
	}

	__int64 value;
	if (ReadProcessMemory(handle, (void*)address, &value, 8, NULL))
	{
		return value;
	}

	CloseHandle(handle);
	return -1;
}

// ok
vector<byte> ReadWrite::ReadBytes( DWORD64 address, INT32 len)
{
	vector<byte> result;

	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, gd.pid);
	if (handle == NULL)
	{
		return result;
	}

	byte* tempResult;
	tempResult = new byte[len];
	memset(tempResult, 0, len);
	ReadProcessMemory(handle, (LPCVOID)address, tempResult, len, NULL);
	result.resize(len);
	for (int i = 0; i < len; i++)
	{
		result[i] = tempResult[i];
	}

	CloseHandle(handle);
	return result;
}

//BYTE ReadWrite::ReadByte( DWORD64 address)
//{
//	BYTE result;
//	ReadBytes(address, &result, sizeof(result));
//	return result;
//}
// ok
FLOAT ReadWrite::ReadFloat( DWORD64 address)
{
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, gd.pid);
	if (handle == NULL)
	{
		return -1;
	}

	float value;
	if (ReadProcessMemory(handle, (void*)address, &value, 4, NULL))
	{
		return value;
	}
	CloseHandle(handle);
	return -1;
}


//BOOL ReadWrite::WriteByte( DWORD64 address, BYTE data)
//{
//	return WriteBytes(address, &data, sizeof(data));
//}

// ok
BOOL ReadWrite::WriteInt( DWORD64 address, DWORD data)
{
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, gd.pid);
	if (handle == NULL)
	{
		return false;
	}

	if (WriteProcessMemory(handle, (void*)address, &data, 4, NULL)) {
		CloseHandle(handle);
		return true;
	}
	return false;
}

// ok
BOOL ReadWrite::WriteLong( DWORD64 address, DWORD64 data)
{
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, gd.pid);
	if (handle == NULL)
	{
		return false;
	}

	bool res = WriteProcessMemory(handle, (void*)address, &data, 8, NULL);
	CloseHandle(handle);
	return res;
}
// ok
BOOL ReadWrite::WriteFloat( DWORD64 address, FLOAT data)
{
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, gd.pid);
	if (handle == NULL)
	{
		return false;
	}
	bool res = WriteProcessMemory(handle, (void*)address, &data, 4, NULL);
	CloseHandle(handle);
	return res;
}

//BOOL ReadWrite::WriteShort( DWORD64 address, WORD data)
//{
//	return WriteBytes(address, &data, sizeof(data));
//}
// 
// ok
BOOL ReadWrite::WriteBytes( ULONG64 address, vector<byte> bytes)
{
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, gd.pid);
	if (handle == NULL)
	{
		return false;
	}

	int length;
	length = (int)bytes.size();
	byte* val = new byte[length];
	for (int i = 0; i < length; i++)
	{
		val[i] = bytes[i];
	}
	bool res = WriteProcessMemory(handle, (LPVOID)address, (LPCVOID)val, length, NULL);
	CloseHandle(handle);
	return res;
}
