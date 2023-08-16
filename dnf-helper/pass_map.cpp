#include "pass_map.h"
#include "common.h"
//#include "action.h"
#include "judge.h"
#include "pack.h"
#include "call.h"
#include "action.h"
#include "simulate.h"
#include "route.h"

PassMap pm;
// �����ͼ
void PassMap::PackPassMap(int direction)
{
	CoordinateStruct current = jd .GetCurrentRoom();
	if (direction == LEFT) // ��
	{
		pk.PackOverMap(current.x - 1, current.y);
	}
	if (direction == RIGHT) // ��
	{
		pk.PackOverMap(current.x + 1, current.y);
	}
	if (direction == UP) // ��
	{
		pk.PackOverMap(current.x, current.y - 1);
	}
	if (direction == DOWN) // ��
	{
		pk.PackOverMap(current.x, current.y + 1);
	}
}

void PassMap::PackPassMapUp()
{
	PackPassMap(UP);
}

void PassMap::PackPassMapLeft()
{
	PackPassMap(LEFT);
}

void PassMap::PackPassMapDown()
{
	PackPassMap(DOWN);
}

void PassMap::PackPassMapRight()
{
	PackPassMap(RIGHT);
}

void PassMap::PassMapProcess(int direction)
{
	if (direction < 0 || direction > 3)
	{
		return;
	}
	if (as.specialProcess) {
		if (as.overMapCnt % 5 == 0 && as.overMapCnt <= 15) {
			pm.PackPassMap(direction);
		}
		return;
	}
	if (as.overMapCnt % 2 == 0 && as.overMapCnt % 30 != 0) {
		// OverMapCall(direction);
		pm.SimulateOverMap(direction);
	}
	// ��������
	else if (as.overMapCnt % 30 == 0) {
		pm.PackPassMap(direction);
	}

}

void PassMap::AutoFindRoute()
{
	CoordinateStruct currentRoom =  jd.GetCurrentRoom();
	CoordinateStruct bossRoom = jd.GetBossRoom();
	if (jd.CoordinateEqual(currentRoom, bossRoom)) {
		return;
	}
	int direction = mapData.GetMapData();
	PassMapProcess(direction);
}



// ��ɪ��ɭ��˳
void FixFindRouteA() {
	CoordinateStruct cr = jd.GetCurrentRoom();
	int x = cr.x;  int y = cr.y;
	int direction = 0;
	if (x == 0 && y == 3) {
		as.pass1 = 0;
		direction = RIGHT;
	}
	if (x == 1 && y == 3) {
		direction = RIGHT;
	}
	if (x == 2 && y == 3) {
		direction = RIGHT;
	}
	if (x == 3 && y == 3) {
		direction = UP;
	}
	if (x == 3 && y == 2) {
		direction = RIGHT;
	}
	if (x == 4 && y == 2) {
		if (as.pass1 == 0) {
			direction = DOWN;
			as.pass1 = 1;
		}
		else
		{
			direction = UP;
		}
	}

	// ���½�
	if (x == 4 && y == 3) {
		direction = RIGHT;
	}
	if (x == 4 && y == 1) {
		direction = UP;
	}
	
	pm.PassMapProcess(direction);

}


//������ɽ˳
void FixFindRouteC() {
	// 0 1 1 1 0 0
	// 1 1 1 1 1 1
	// 0 1 1 1 0 0
	CoordinateStruct cr = jd.GetCurrentRoom();
	int x = cr.x;  
	int y = cr.y;
	int direction = 0;
	if (x == 0 && y == 1) {
		as.pass1 = 0;
		as.pass2 = 0;
		direction = RIGHT;
	}
	// ѭ��1
	if (x == 1 && y == 1) {
		if (as.pass1 == 0)
		{
			direction = RIGHT;
			as.pass1 = 1;
		}else if(as.pass1 == 1)
		{
			direction = DOWN;
		}
	}
	// ѭ��2
	if (x == 3 && y == 1) {
		if (as.pass2 == 0)
		{
			direction = UP;
			as.pass2 = 1;
		}else if(as.pass2 == 1)
		{
			direction = RIGHT;
		}
	}
	// ��һ�� ���Ͻ�
	if (y == 0 && x == 1) {
		as.pass1 = 1;
		direction = DOWN;
	}
	// ��һ������
	if (y == 0 && (x == 2 || x == 3)) {
		direction = LEFT;
	}
	// ���²� && ���½�
	if (y == 2 && x == 3) {
		direction = UP;
	}
	// ���²�
	if (y == 2 && (x == 1 || x == 2)) {
		direction = RIGHT;
	}
	// �м��
	if (y == 1 && x == 2) {
		direction = RIGHT;
	}
	if (y == 1 && x == 4)
	{
		direction = RIGHT;
	}
	
	pm.PassMapProcess(direction);

}



void PassMap::FixFindRoute()
{
	int mapId = cl.GetMapId();
	if (mapId == 100002676 || mapId == 400001567) {
		FixFindRouteA();
		return;
	}
	if (mapId == 100002962 || mapId == 100002963) {
		FixFindRouteC();
		return;
	}
	AutoFindRoute();
}

void PassMap::SimulateOverMap(int direction)
{
	if (!jd.IsAtMap())
	{
		return;
	}
	if (!jd.IsOpenDoor())
	{
		return;
	}
	ULONG64 mapOffset = rw.ReadLong(gd.personPtr + ��ͼƫ��);
	if (mapOffset == 0) {
		return;
	}

	ULONG64 roomData = rw.ReadLong(rw.ReadLong(rw.ReadLong(������) + ʱ���ַ) + ˳ͼƫ��);
	ULONG64	coordinateStructure = roomData + direction * ����ID + ����˳ͼ;
	int	startX = rw.ReadInt(coordinateStructure + 0);
	int	startY = rw.ReadInt(coordinateStructure + 4);
	int	endX = rw.ReadInt(coordinateStructure + 8);
	int	endY = rw.ReadInt(coordinateStructure + 12);
	int x = 0, y = 0;

	if (direction == 0){
		x = int(startX + endX + 20);
		y = int(startY + endY / 2);
	}
	if (direction == 1) {
		x = int(startX - 20);
		y = int(startY + endY / 2);
	}
	if (direction == 2) {
		x = int(startX + endX / 2);
		y = int(startY + endY + 20);
	}
	if (direction == 3) {
		x = int(startX + endX / 2);
		y = int(startY - 20);
	}
	CoordinateStruct coo = jd.GetCurrentRoom();
	sl.GoDestation(x, y, coo, true);
	Sleep(100);
	// ��ֹ�ѹ�ͼִ����Чģ��
	if(jd.CoordinateEqual(coo, jd.GetCurrentRoom())){
		sl.GoDestation(int(startX + endX / 2), startY, jd.GetCurrentRoom(), true);
	}
}
