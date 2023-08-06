#include "common.h"


// ÔËËã·ûÖØÔØ
vector<byte> operator+(vector<byte> a, vector<byte> b)
{
	for (int i = 0; i < b.size(); i++)
	{
		a.push_back(b[i]);
	}

	b.clear();

	return a;
}



