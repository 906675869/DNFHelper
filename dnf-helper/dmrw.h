#include <Windows.h>
#include <vector>
using namespace std;

class DmReadWrite {
public :
	LPVOID ApplyMemory(DWORD len);

	DWORD ReadInt(DWORD64 address);

	DWORD64 ReadLong(DWORD64 address);

	vector<BYTE> ReadBytes(DWORD64 address, INT32 len);

	FLOAT ReadFloat(DWORD64 address);

	BOOL WriteInt(DWORD64 address, DWORD data);

	BOOL WriteLong(DWORD64 address, DWORD64 data);

	BOOL WriteFloat(DWORD64 address, FLOAT data);

	BOOL WriteBytes(ULONG64 address, vector<BYTE> bytes);


};