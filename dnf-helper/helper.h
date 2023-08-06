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

// 字节累加
vector<BYTE> AppendBytes(vector<BYTE> oldData, vector<BYTE> newData);

// 整数转字节数组
vector<BYTE> LongToBytes(ULONG64 data);

void CreateUserThead(LPVOID method);

// 获取空白字节集
vector<BYTE> GetEmptyBytes(int num);

// 创建线程

void CreateUserThead(LPVOID method);
// 通过进程名查进程id
BOOL FindProcessPid(LPCWSTR ProcessName, DWORD& dwPid);

// 创建字节数组
vector<byte> makeByteArray(vector<byte> data);
