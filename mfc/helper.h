#pragma once
#include <windows.h>
#include <afxstr.h>
#include <afxcmn.h>
#include <vector>
#include <string>

using namespace std;

namespace game{
	// ͨ�������������id
	BOOL FindProcessPid(LPCWSTR ProcessName, DWORD& dwPid);

	// ����̨��־
	void CmfcLog(CString logstr);


	extern CEdit* logEdit;


	std::vector<byte> IntToBytes(ULONG64  i, int size = 0);


	int GetRandomNum(int min, int max);

	

	vector<byte> AnsiToUnicode(string str);

	wstring UnicodeToAnsi(vector<byte> bytes);

	wstring GetTextRight(wstring text, size_t size);

	wstring GetTextLeft(wstring text, size_t size);

	wstring PassMapTime(int address);

	void SplitText(const wstring& str, vector<wstring>& tokens, const wstring& delimiters);

	// �ֽ��ۼ�
	vector<BYTE> AppendBytes(vector<BYTE> oldData, vector<BYTE> newData);

	// ����ת�ֽ�����
	vector<BYTE> LongToBytes(ULONG64 data);

	void CreateUserThead(LPVOID method);

	// ��ȡ�հ��ֽڼ�
	vector<BYTE> GetEmptyBytes(int num);


	// �����ֽ�����
	vector<byte> makeByteArray(vector<byte> data);

	// wstring to string 
	wstring string2wstring(string str);

	string wstring2string(wstring wstr);


}