#include <fstream>
#include<atlstr.h>
#include <ShlObj.h>
#include <time.h>
#include "config.h"

ConfigData configData;
Config config;

wstring Config::GetDesktopPath()
{
	LPWSTR path[255];
	ZeroMemory(path, 255);
	SHGetSpecialFolderPath(0, (LPWSTR)path, CSIDL_DESKTOPDIRECTORY, 0);
	wstring lp = ((LPWSTR)path);
	wstring sp = L"\\";
	lp = lp + sp;
	return 	lp;
}

void Config::WriteConfigItem(wstring itemName, wstring data)
{
	wstring fPath = GetDesktopPath() + L"\\" + configData.appName + L".ini";
	wstring nodeName = configData.appName + L"����";
	WritePrivateProfileStringW(nodeName.c_str(), itemName.c_str(), data.c_str(), fPath.c_str());
}

int Config::ReadConfigItem(wstring itemName)
{
	wstring fPath = GetDesktopPath() + L"\\" + configData.appName + L".ini";
	wstring nodeName = configData.appName + L"����";
	return GetPrivateProfileIntW(nodeName.c_str(), itemName.c_str(), 0, fPath.c_str());
}

bool Config::FileExists()
{
	wstring fPath = GetDesktopPath() + L"\\" + configData.appName + L".ini";
	if (GetFileAttributesW(fPath.c_str()) == INVALID_FILE_ATTRIBUTES)return false;
	else return true;
}

void Config::ConfigInit()
{
	configData.appName = L"̰��";


	configData.skillCode = L"���ܴ���";
	configData.skillHarm = L"�����˺�";
	configData.skillRate = L"����Ƶ��";
	configData.skillNum = L"���ܸ���";
	configData.skillSize = L"���ܴ�С";

	configData.autoModel = L"�Զ�ģʽ";
	configData.mapId = L"�������";
	configData.mapLevel = L"�����Ѷ�";

	configData.leftFatigue = L"Ԥ��ƣ��";
	configData.roleNum = L"ˢͼ��ɫ";

	configData.followModel = L"���淽ʽ";
	configData.pickupType = L"ʰȡ��ʽ";
	configData.passMapType = L"˳ͼ��ʽ";

	configData.goodsBuff = L"��ƷBuff";
	configData.superBuff = L"����buff";
	configData.inviciable = L"�����޵�";
	configData.goldPerson = L"С����";

	configData.whiteEquip = L"��ɫװ��";
	configData.blueEquip = L"��ɫװ��";
	configData.purpleEquip = L"��ɫװ��";
	configData.pinkEquip = L"��ɫװ��";
	configData.SSEquip = L"ʷʫװ��";
	configData.braveEquip = L"����װ��";
	configData.CsEquip = L"��˵װ��";

	configData.activityRole = L"���ɫ";

	configData.δ��ȫͼ = L"δ��ȫͼ";
}

void Config::WriteConfigFile()
{
	if (FileExists()) {
		return;
	}

	WriteConfigItem(configData.skillCode, L"109006107");
	WriteConfigItem(configData.skillHarm, L"85000000");
	WriteConfigItem(configData.skillRate, L"300");
	WriteConfigItem(configData.skillNum, L"2");
	WriteConfigItem(configData.skillSize, L"5\n");

	WriteConfigItem(configData.autoModel, L"1		={1}ָ����ͼ ={2}�������� ={3}��Ӧ�ȼ���ͼ ={4}���� ={5}����δ��");
	WriteConfigItem(configData.mapId, L"100002676");
	WriteConfigItem(configData.mapLevel, L"5		={0}��ͨ�� ={1}ð�ռ� ={2}��ʿ�� ={3}���߼� ={4}ج�μ� ={5}ȡ���\n");

	WriteConfigItem(configData.leftFatigue, L"8");
	WriteConfigItem(configData.roleNum, L"3");

	WriteConfigItem(configData.followModel, L"1		={0}��ִ�� ={1}������� ={2}Ư�Ƹ���");
	WriteConfigItem(configData.pickupType, L"3		={0}��ִ�� ={1}������� ={2}������ ={3}�������");
	WriteConfigItem(configData.passMapType, L"2		={0}��ִ�� ={1}����˳ͼ ={2}Ư��˳ͼ ={3}ǿ��˳ͼ");
	WriteConfigItem(configData.goodsBuff, L"0		={0}�� ={1}��\n");
	WriteConfigItem(configData.superBuff, L"0		={0}�� ={1}��\n");
	WriteConfigItem(configData.inviciable, L"0		={0}�� ={1}��\n");
	WriteConfigItem(configData.goldPerson, L"1		={0}�� ={1}��\n");
	WriteConfigItem(configData.δ��ȫͼ, L"0		={0}�� ={1}��\n");

	WriteConfigItem(configData.whiteEquip, L"0		={0}��ִ�� ={1}���� ={2}�ֽ� ={3}����");
	WriteConfigItem(configData.blueEquip, L"0		={0}��ִ�� ={1}���� ={2}�ֽ� ={3}����");
	WriteConfigItem(configData.purpleEquip, L"0		={0}��ִ�� ={1}���� ={2}�ֽ� ={3}����");
	WriteConfigItem(configData.pinkEquip, L"0		={0}��ִ�� ={1}���� ={2}�ֽ� ={3}����");
	WriteConfigItem(configData.SSEquip, L"0		={0}��ִ�� ={1}���� ={2}�ֽ� ={3}����");
	WriteConfigItem(configData.braveEquip, L"0		={0}��ִ�� ={1}���� ={2}�ֽ� ={3}����");
	WriteConfigItem(configData.CsEquip, L"0		={0}��ִ�� ={1}���� ={2}�ֽ� ={3}����\n");

	WriteConfigItem(configData.activityRole, L"0  ={0} �ǻ��ɫ ={1} ���ɫ");
}
