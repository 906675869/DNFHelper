#include "pch.h"
#include "dmrw.h"
#include "dm.h"
#include "helper.h"
#include "config.h"
#include <string>


LPVOID game::DmReadWrite::ApplyMemory(DWORD len)
{
	g_dm->SetMemoryHwndAsProcessId(game::processConfig.gamePid);
	return (LPVOID)g_dm->VirtualAllocEx(game::processConfig.gamePid, 0, len, 0);
}

DWORD game::DmReadWrite::ReadInt(DWORD64 address)
{
	g_dm->SetMemoryHwndAsProcessId(game::processConfig.gamePid);
	return g_dm->ReadIntAddr(game::processConfig.gamePid, address, 4);
}

DWORD64 game::DmReadWrite::ReadLong(DWORD64 address)
{
	g_dm->SetMemoryHwndAsProcessId(game::processConfig.gamePid);
	return g_dm->ReadIntAddr(game::processConfig.gamePid, address, 3);
}

CString game::DmReadWrite::ReadBytes(DWORD64 address, INT32 len)
{
	g_dm->SetMemoryHwndAsProcessId(game::processConfig.gamePid);
	return g_dm->ReadDataAddr(game::processConfig.gamePid, address, len);
}

FLOAT game::DmReadWrite::ReadFloat(DWORD64 address)
{
	g_dm->SetMemoryHwndAsProcessId(game::processConfig.gamePid);
	return g_dm->ReadFloatAddr(game::processConfig.gamePid, address);
}

BOOL game::DmReadWrite::WriteInt(DWORD64 address, DWORD data)
{
	g_dm->SetMemoryHwndAsProcessId(game::processConfig.gamePid);
	return g_dm->WriteIntAddr(game::processConfig.gamePid, address, 4, data);
}

BOOL game::DmReadWrite::WriteLong(DWORD64 address, DWORD64 data)
{
	g_dm->SetMemoryHwndAsProcessId(game::processConfig.gamePid);
	return g_dm->WriteIntAddr(game::processConfig.gamePid, address, 3, data);
}

BOOL game::DmReadWrite::WriteFloat(DWORD64 address, FLOAT data)
{
	g_dm->SetMemoryHwndAsProcessId(game::processConfig.gamePid);
	return g_dm->WriteFloatAddr(game::processConfig.gamePid, address, data);
}

BOOL game::DmReadWrite::WriteBytes(ULONG64 address, CString bytes)
{
	g_dm->SetMemoryHwndAsProcessId(game::processConfig.gamePid);
	return g_dm->WriteDataAddr(game::processConfig.gamePid, address, bytes);
}
