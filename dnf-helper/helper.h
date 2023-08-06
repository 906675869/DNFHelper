#pragma once
#include "common.h"

vector<BYTE> IntToBytes(ULONG64  i, int size = 0);

int GetRandomNum(int min, int max);

wstring ToText(int val);

::vector<BYTE> AnsiToUnicode(string str);

wstring UnicodeToAnsi(vector<BYTE> bytes);

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

// �����߳�

void CreateUserThead(LPVOID method);
// ͨ�������������id
BOOL FindProcessPid(LPCWSTR ProcessName, DWORD& dwPid);

// �����ֽ�����
vector<byte> makeByteArray(vector<byte> data);
