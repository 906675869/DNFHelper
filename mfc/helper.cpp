#include "pch.h"
#include "helper.h"
#include <stdio.h>
#include <tlhelp32.h> 
#include <vector>
#include <string>

using namespace std;

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


std::vector<byte> game::IntToBytes(ULONG64  i, int size)
{
	if (size == 0)
	{
		size = 8;
	}
	ULONG64  adr = (ULONG64)&i;
	std::vector<BYTE>c;
	for (size_t i = 0; i < size; i++) {
		c.push_back(*(BYTE*)adr++);
	}
	return c;
}


int game::GetRandomNum(int min, int max)
{
	int rNum = 0;
	srand(GetTickCount());
	for (size_t i = 0; i < 31; i++)
		rNum |= (rand() & 1) << i;
	return min + rNum % (max - min + 1);
}



std::vector<byte> game::AnsiToUnicode(string str)
{
	std::vector<BYTE>Ret;
	DWORD dwNum = MultiByteToWideChar(936, 0, str.c_str(), -1, NULL, 0);
	BYTE* pwText;
	pwText = new  BYTE[dwNum * 2];
	MultiByteToWideChar(936, 0, str.c_str(), -1, (LPWSTR)pwText, dwNum * 2);

	for (size_t i = 0; i < dwNum * 2; i++)
	{
		Ret.push_back(pwText[i]);
	}
	Ret.push_back(0);
	Ret.push_back(0);
	return Ret;
}

wstring game::UnicodeToAnsi(std::vector<BYTE> bytes)
{
	size_t bytesLen = bytes.size();
	CHAR* local_Unicode = new CHAR[bytesLen];
	for (size_t i = 0; i < bytesLen; i++)
	{
		local_Unicode[i] = bytes[i];
	}
	//UniCode到Ansi
	DWORD local_num = WideCharToMultiByte(936, 512, (LPCWCH)local_Unicode, -1, 0, 0, 0, FALSE);
	CHAR* buff = new CHAR[local_num];
	WideCharToMultiByte(936, 512, (LPCWCH)local_Unicode, -1, buff, local_num, NULL, NULL);

	// string转wstring
	DWORD nLen = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, buff, -1, NULL, 0);
	if (nLen == 0)
	{
		return NULL;
	}
	WCHAR* pResult = new WCHAR[nLen];
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, buff, -1, pResult, nLen);

	return pResult;
}



wstring game::GetTextRight(wstring text, size_t size)
{
	wstring result;

	if (size > text.size())
	{
		size = 0;
	}
	else
	{
		size = text.size() - size;
	}

	result = text.substr(size);

	return result;
}

wstring GetTextLeft(wstring text, size_t size)
{
	wstring result;

	if (size > text.size())
	{
		size = text.size();
	}

	result = text.substr(0, size);

	return result;
}

wstring game::PassMapTime(int address)
{
	int i1;
	int i2;
	int i3;
	wstring i4;
	wstring i5;

	if (address < 1000)
	{
		i5 = L"0秒:" + ToText(address / 10);
	}
	else
	{
		i4 = GetTextRight(ToText(address), 3);
		i4 = GetTextLeft(i4, 2);

		address = address / 1000;

		i1 = address / 3600;
		i2 = (address - i1 * 3600) / 60;
		i3 = (address - i1 * 3600) % 60;

		if (i1 > 0)
		{
			i5 = ToText(i1) + L"时:" + ToText(i2) + L"分:" + ToText(i3) + L"秒:" + i4;
		}
		else if (i2 > 0)
		{
			i5 = ToText(i2) + L"分:" + ToText(i3) + L"秒:" + i4;
		}
		else if (i3 > 0)
		{
			i5 = ToText(i3) + L"秒:" + i4;
		}
	}
	return i5;
}

void game::SplitText(const wstring& str, vector<wstring>& tokens, const wstring& delimiters)
{
	// Skip delimiters at beginning.
	wstring::size_type lastPos = str.find_first_not_of(delimiters, 0);
	// Find first "non-delimiter".
	wstring::size_type pos = str.find_first_of(delimiters, lastPos);
	while (wstring::npos != pos || wstring::npos != lastPos)
	{
		// Found a token, add it to the vector.
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		// Skip delimiters.Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);
		// Find next "non-delimiter"
		pos = str.find_first_of(delimiters, lastPos);
	}
}



ULONG64 game::HexStrToLong(string hexStr)
{
	ULONG64 retn = 0;

	string Hex = "0123456789ABCDEF";

	for (size_t i = 0; i < hexStr.size(); i++)
	{
		size_t Index = Hex.find(hexStr.substr(i, 1));

		if (Index != Hex.npos)
		{
			retn = retn * 16 + Index;
		}
	}
	return retn;
}

string game::LongToHexStr(long long num)
{
	string str;
	long long temp = num / 16;
	int left = num % 16;
	if (temp > 0)
	{
		str += game::LongToHexStr(temp);
	}
	if (left < 10)
	{
		str += (left + '0');
	}
	else
	{
		str += ('A' + left - 10);
	}
	return str;
}



// 字节相加
vector<BYTE> game::AppendBytes(vector<BYTE> oldData, vector<BYTE> newData) {
	vector<BYTE> addr(oldData.size() + newData.size());
	for (size_t i = 0; i < oldData.size(); i++) {
		addr[i] = oldData[i];
	}
	for (size_t i = 0; i < newData.size(); i++) {
		addr[oldData.size() + i] = newData[i];
	}
	return addr;
}

// 整数转字节数组
vector<BYTE> LongToBytes(ULONG64 data) {
	size_t size = sizeof(data);
	vector<BYTE> ret(size);
	memcpy(ret.data(), &data, size);
	return ret;
}



// 获取空白字节集
vector<BYTE> GetEmptyBytes(int num)
{
	vector<BYTE> res;
	for (size_t i = 0; i < num; i++)
	{
		res.push_back(0);
	}
	return res;
}




// 创建字节数组
vector<byte> game::makeByteArray(vector<byte> data)
{
	return data;
}

//将string转换成wstring  
wstring game::string2wstring(string str)
{
	wstring result;
	//获取缓冲区大小，并申请空间，缓冲区大小按字符计算  
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
	TCHAR* buffer = new TCHAR[len + 1];
	//多字节编码转换成宽字节编码  
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
	buffer[len] = '\0';             //添加字符串结尾  
	//删除缓冲区并返回值  
	result.append(buffer);
	delete[] buffer;
	return result;
}

//将wstring转换成string  
string game::wstring2string(wstring wstr)
{
	string result;
	//获取缓冲区大小，并申请空间，缓冲区大小事按字节计算的  
	int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
	char* buffer = new char[len + 1];
	//宽字节编码转换成多字节编码  
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
	buffer[len] = '\0';
	//删除缓冲区并返回值  
	result.append(buffer);
	delete[] buffer;
	return result;
}
