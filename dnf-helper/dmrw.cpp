#include "dmrw.h"
#include "dm.h"
#include "common.h"
#include "helper.h"

LPVOID DmReadWrite::ApplyMemory(DWORD len)
{
	g_dm->SetMemoryHwndAsProcessId(gd.pid);
	return (LPVOID)g_dm->VirtualAllocEx(gd.pid, 0, len, 0);
}

DWORD DmReadWrite::ReadInt(DWORD64 address)
{
	g_dm->SetMemoryHwndAsProcessId(gd.pid);
	return g_dm->ReadIntAddr(gd.pid, address, 4);
}

DWORD64 DmReadWrite::ReadLong(DWORD64 address)
{
	g_dm->SetMemoryHwndAsProcessId(gd.pid);
	return g_dm->ReadIntAddr(gd.pid, address, 3);
}

vector<BYTE> DmReadWrite::ReadBytes(DWORD64 address, INT32 len)
{
	g_dm->SetMemoryHwndAsProcessId(gd.pid);
	string data = g_dm->ReadDataAddr(gd.pid, address, len);
	return HexToBytes(data);
}

FLOAT DmReadWrite::ReadFloat(DWORD64 address)
{
	g_dm->SetMemoryHwndAsProcessId(gd.pid);
	return g_dm->ReadFloatAddr(gd.pid, address);
}

BOOL DmReadWrite::WriteInt(DWORD64 address, DWORD data)
{
	g_dm->SetMemoryHwndAsProcessId(gd.pid);
	return g_dm->WriteIntAddr(gd.pid, address, 4, data);
}

BOOL DmReadWrite::WriteLong(DWORD64 address, DWORD64 data)
{
	g_dm->SetMemoryHwndAsProcessId(gd.pid);
	return g_dm->WriteIntAddr(gd.pid, address, 3, data);
}

BOOL DmReadWrite::WriteFloat(DWORD64 address, FLOAT data)
{
	g_dm->SetMemoryHwndAsProcessId(gd.pid);
	return g_dm->WriteFloatAddr(gd.pid, address, data);
}

BOOL DmReadWrite::WriteBytes(ULONG64 address, vector<BYTE> bytes)
{
	string data = BytesToHexStr(bytes);
	g_dm->SetMemoryHwndAsProcessId(gd.pid);
	wstring wdata = string2wstring(data);
	return g_dm->WriteDataAddr(gd.pid, address, wdata.c_str());
}
