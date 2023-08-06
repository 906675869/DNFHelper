#pragma once
#include "common.h"

class Config {
public:
	// ��ȡ�����ַ
	wstring GetDesktopPath();

	void WriteConfigItem(wstring itemName, wstring data);

	int ReadConfigItem(wstring itemName);

	bool FileExists();

	//LPWSTR ReadConfigA(LPCWSTR configItem);

	void ConfigInit();

	void WriteConfigFile();
};

extern Config config;