#include "common.h"

// 运算符重载
vector<byte> operator+(vector<byte> a, vector<byte> b)
{
	for (int i = 0; i < b.size(); i++)
	{
		a.push_back(b[i]);
	}

	b.clear();

	return a;
}


// byte数组转16进制字符串：带空格
string BytesToHexStr(vector<byte> bytes)
{
	string buff = "";
	for (int i = 0; i < bytes.size(); i++)
	{
		int high = bytes[i] / 16;
		int low = bytes[i] % 16;
		buff += (high < 10) ? ('0' + high) : ('A' + high - 10);
		buff += (low < 10) ? ('0' + low) : ('A' + low - 10);
		if (i != bytes.size() - 1)
		{
			buff += " ";
		}
	}
	return buff;
}

string remove_space(string str)
{
	str.erase(remove_if(str.begin(), str.end(), ::isspace), str.end());
	return str;
}

// 16进制字符串转byte数组： 
vector<byte> HexToBytes(const string hex)
{
	string hex1 = remove_space(hex);
	int len = (int)hex1.length() / 2;
	vector<byte> bytes;
	bytes.resize(len);
	string strByte;
	unsigned int num;
	for (__int64 i = 0; i < len; i++)
	{
		strByte = hex1.substr(i * 2, 2);
		sscanf_s(strByte.c_str(), "%x", &num);
		bytes[i] = num;
	}
	return bytes;
}