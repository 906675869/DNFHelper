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
	wstring nodeName = configData.appName + L"配置";
	WritePrivateProfileStringW(nodeName.c_str(), itemName.c_str(), data.c_str(), fPath.c_str());
}

int Config::ReadConfigItem(wstring itemName)
{
	wstring fPath = GetDesktopPath() + L"\\" + configData.appName + L".ini";
	wstring nodeName = configData.appName + L"配置";
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
	configData.appName = L"贪狼";


	configData.skillCode = L"技能代码";
	configData.skillHarm = L"技能伤害";
	configData.skillRate = L"技能频率";
	configData.skillNum = L"技能个数";
	configData.skillSize = L"技能大小";

	configData.autoModel = L"自动模式";
	configData.mapId = L"副本编号";
	configData.mapLevel = L"副本难度";

	configData.leftFatigue = L"预留疲劳";
	configData.roleNum = L"刷图角色";

	configData.followModel = L"跟随方式";
	configData.pickupType = L"拾取方式";
	configData.passMapType = L"顺图方式";

	configData.goodsBuff = L"物品Buff";
	configData.superBuff = L"超级buff";
	configData.inviciable = L"霸体无敌";
	configData.goldPerson = L"小金人";

	configData.whiteEquip = L"白色装备";
	configData.blueEquip = L"蓝色装备";
	configData.purpleEquip = L"紫色装备";
	configData.pinkEquip = L"粉色装备";
	configData.SSEquip = L"史诗装备";
	configData.braveEquip = L"勇者装备";
	configData.CsEquip = L"传说装备";

	configData.activityRole = L"活动角色";

	configData.未央全图 = L"未央全图";
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

	WriteConfigItem(configData.autoModel, L"1		={1}指定地图 ={2}剧情升级 ={3}适应等级地图 ={4}暂无 ={5}云上未央");
	WriteConfigItem(configData.mapId, L"100002676");
	WriteConfigItem(configData.mapLevel, L"5		={0}普通级 ={1}冒险级 ={2}勇士级 ={3}王者级 ={4}噩梦级 ={5}取最高\n");

	WriteConfigItem(configData.leftFatigue, L"8");
	WriteConfigItem(configData.roleNum, L"3");

	WriteConfigItem(configData.followModel, L"1		={0}不执行 ={1}坐标跟随 ={2}漂移跟随");
	WriteConfigItem(configData.pickupType, L"3		={0}不执行 ={1}按键入包 ={2}落地入包 ={3}空中入包");
	WriteConfigItem(configData.passMapType, L"2		={0}不执行 ={1}坐标顺图 ={2}漂移顺图 ={3}强制顺图");
	WriteConfigItem(configData.goodsBuff, L"0		={0}否 ={1}是\n");
	WriteConfigItem(configData.superBuff, L"0		={0}否 ={1}是\n");
	WriteConfigItem(configData.inviciable, L"0		={0}否 ={1}是\n");
	WriteConfigItem(configData.goldPerson, L"1		={0}否 ={1}是\n");
	WriteConfigItem(configData.未央全图, L"0		={0}否 ={1}是\n");

	WriteConfigItem(configData.whiteEquip, L"0		={0}不执行 ={1}出售 ={2}分解 ={3}丢弃");
	WriteConfigItem(configData.blueEquip, L"0		={0}不执行 ={1}出售 ={2}分解 ={3}丢弃");
	WriteConfigItem(configData.purpleEquip, L"0		={0}不执行 ={1}出售 ={2}分解 ={3}丢弃");
	WriteConfigItem(configData.pinkEquip, L"0		={0}不执行 ={1}出售 ={2}分解 ={3}丢弃");
	WriteConfigItem(configData.SSEquip, L"0		={0}不执行 ={1}出售 ={2}分解 ={3}丢弃");
	WriteConfigItem(configData.braveEquip, L"0		={0}不执行 ={1}出售 ={2}分解 ={3}丢弃");
	WriteConfigItem(configData.CsEquip, L"0		={0}不执行 ={1}出售 ={2}分解 ={3}丢弃\n");

	WriteConfigItem(configData.activityRole, L"0  ={0} 非活动角色 ={1} 活动角色");
}
