#include "pack.h"
#include "helper.h"
#include "call.h"

PackFunc pk;

static vector<BYTE> packData;

static bool syncFlag;

// 缓冲
void BufferCall(ULONG64 params) {
	packData = AppendBytes(packData, { 72, 131, 236, 96 });

	packData = AppendBytes(packData, { 72, 185 });
	packData = AppendBytes(packData, LongToBytes(发包基址));

	packData = AppendBytes(packData, { 186 });
	packData = AppendBytes(packData, IntToBytes(params, 4));


	packData = AppendBytes(packData, { 72, 184 });
	packData = AppendBytes(packData, LongToBytes(缓冲CALL));

	packData = AppendBytes(packData, { 255, 208 });
}
// 加密
void EncryptCall(ULONG64 params, int size) {
	packData = AppendBytes(packData, { 72, 185 });
	packData = AppendBytes(packData, LongToBytes(发包基址));

	packData = AppendBytes(packData, { 72, 186 });
	packData = AppendBytes(packData, LongToBytes(params));

	packData = AppendBytes(packData, { 72, 184 });
	if (size == 1) {
		packData = AppendBytes(packData, LongToBytes(加密包CALL));
	}
	if (size == 2) {
		packData = AppendBytes(packData, LongToBytes(加密包CALL2));
	}
	if (size == 4) {
		packData = AppendBytes(packData, LongToBytes(加密包CALL4));
	}
	if (size == 8) {
		packData = AppendBytes(packData, LongToBytes(加密包CALL8));
	}
	packData = AppendBytes(packData, { 255, 208 });

}
// 发包
void SendCall() {
	packData = AppendBytes(packData, { 72, 184 });
	packData = AppendBytes(packData, LongToBytes(发包CALL));

	packData = AppendBytes(packData, { 255, 208 });
	packData = AppendBytes(packData, { 72, 131, 196, 96 });
	cl.MemoryCompileCall(packData);
	packData.clear();
}

void PackFunc::Move(int worldMapId, int townMapId, int townX, int townY) {
	BufferCall(36);
	EncryptCall(worldMapId, 4);
	EncryptCall(townMapId, 4);
	EncryptCall(townX, 2);
	EncryptCall(townY, 2);
	EncryptCall(5, 1);
	EncryptCall(38, 4);
	EncryptCall(0, 2);
	EncryptCall(0, 4);
	EncryptCall(0, 1);
	SendCall();

}

void PackFunc::FlopCard(int x, int y)
{
	BufferCall(69);
	SendCall();
	BufferCall(70);
	SendCall();
	BufferCall(71);
	EncryptCall(x, 1);
	EncryptCall(y, 1);
	SendCall();

	BufferCall(1426);
	SendCall();

}

void PackFunc::PassRoom(int x, int y)
{
	BufferCall(45);
	EncryptCall(x, 1);
	EncryptCall(y, 1);
	EncryptCall(0, 4);
	EncryptCall(0, 4);
	EncryptCall(0, 1);

	for (int i = 0; i < 9; i++)
	{
		EncryptCall(0, 2);
	}

	for (int i = 0; i < 8; i++)
	{
		EncryptCall(0, 4);
	}

	for (int i = 0; i < 7; i++)
	{
		EncryptCall(0, 2);
	}

	EncryptCall(0, 4);

	EncryptCall(0, 2);
	EncryptCall(0, 2);
	EncryptCall(0, 2);

	EncryptCall(0, 4);
	EncryptCall(0, 4);
	EncryptCall(0, 4);

	EncryptCall(0, 2);
	EncryptCall(0, 1);
	SendCall();
}

void PackFunc::GetinMap(int mapId, int mapLevel, int abyss, int practice)
{
	BufferCall(16);
	EncryptCall(mapId, 4);
	EncryptCall(mapLevel, 1);
	EncryptCall(0, 2);
	EncryptCall(abyss, 1);
	EncryptCall(practice, 1);
	EncryptCall(65535, 2);
	EncryptCall(0, 4);
	EncryptCall(1, 1);
	EncryptCall(0, 4);
	EncryptCall(0, 1);
	EncryptCall(-1, 4);
	SendCall();
}

void PackFunc::ChooseMap()
{
	BufferCall(15);
	EncryptCall(0, 4);
	SendCall();
}


void PackFunc::PackPickup(int code)
{
	BufferCall(43);
	EncryptCall(code, 4);
	EncryptCall(0, 1);
	EncryptCall(1, 1);
	EncryptCall(566, 2);
	EncryptCall(291, 2);
	EncryptCall(9961, 2);
	EncryptCall(553, 2);
	EncryptCall(285, 2);
	EncryptCall(18802, 2);
	EncryptCall(24743, 2);
	SendCall();
}



void PackFunc::PackOverMap(int x, int y)
{
	if (x < 0 || y < 0) {
		return;
	}
	BufferCall(45);
	EncryptCall(x, 1);
	EncryptCall(y, 1);
	EncryptCall(0, 4);
	EncryptCall(0, 4);
	EncryptCall(0, 1);

	for (int i = 0; i < 9; i++)
	{
		EncryptCall(0, 2);
	}

	for (int i = 0; i < 8; i++)
	{
		EncryptCall(0, 4);
	}

	for (int i = 0; i < 7; i++)
	{
		EncryptCall(0, 2);
	}

	EncryptCall(0, 4);

	EncryptCall(0, 2);
	EncryptCall(0, 2);
	EncryptCall(0, 2);

	EncryptCall(0, 4);
	EncryptCall(0, 4);
	EncryptCall(0, 4);

	EncryptCall(0, 2);
	EncryptCall(0, 1);


	SendCall();
}

void PackFunc::PackSale(int saleSize, vector<int> postions)
{
	BufferCall(22);
	EncryptCall(317, 4);
	EncryptCall(95, 4);
	EncryptCall(saleSize, 1);
	for (int i = 0; i < saleSize; i++) {
		EncryptCall(0, 1);
		EncryptCall(postions.at(i), 2);
		EncryptCall(1, 4);
		EncryptCall(postions.at(i) * 2 + 2, 4);
	}
	SendCall();
}

void PackFunc::PackDecompose(int postion)
{
	BufferCall(26);
	EncryptCall(0, 1);
	EncryptCall(65535, 2);
	EncryptCall(317, 4);
	EncryptCall(1, 1);
	EncryptCall(postion, 2);
	SendCall();
}



void PackFunc::RoleList()
{
	BufferCall(7);
	SendCall();
}

void PackFunc::ChooseRole(int position)
{
	BufferCall(4);
	EncryptCall(position, 4);
	SendCall();
}

void PackFunc::ContinueChangle()
{
	BufferCall(72);
	EncryptCall(1, 1);
	EncryptCall(0, 1);
	EncryptCall(1, 1);
	SendCall();
}

void PackFunc::OutMap()
{
	BufferCall(42);
	SendCall();
}

void PackFunc::TidyupBag(int bagtab, int bagtype, int sorttype)
{
	BufferCall(20);
	EncryptCall(6, 4);
	EncryptCall(16, 1);
	EncryptCall(bagtype, 1);
	EncryptCall(24, 1);
	EncryptCall(bagtab, 1);
	EncryptCall(32, 1);
	EncryptCall(sorttype, 1);
	SendCall();
}
