#pragma once

class FastCall {
public:
	FastCall() {
		InitCode();
	}
	~FastCall() {
		FreeCode();
	}
	// ≥ı ºªØ
	static void InitCode();
	static void FreeCode();
	static ULONG64 AllocateSpace(int len);
	static void InitHookType(int interfaceSelect);
	static void CallWait();
	static ULONG64 CallFunctionAutoFindStack(vector<byte> callData, int rsp = NULL);
	static ULONG64 MemoryCompileCall(vector<byte> callData);
	static ULONG64 Call(ULONG64 address, vector<ULONG64> data);
};


