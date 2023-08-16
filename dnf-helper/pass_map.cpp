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
// 组包过图
void PassMap::PackPassMap(int direction)
{
	CoordinateStruct current = jd .GetCurrentRoom();
	if (direction == LEFT) // 左
	{
		pk.PackOverMap(current.x - 1, current.y);
	}
	if (direction == RIGHT) // 右
	{
		pk.PackOverMap(current.x + 1, current.y);
	}
	if (direction == UP) // 上
	{
		pk.PackOverMap(current.x, current.y - 1);
	}
	if (direction == DOWN) // 下
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
	// 卡包处理
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



// 纳瑟乌森林顺
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

	// 右下角
	if (x == 4 && y == 3) {
		direction = RIGHT;
	}
	if (x == 4 && y == 1) {
		direction = UP;
	}
	
	pm.PassMapProcess(direction);

}


//柯涅恩山顺
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
	// 循环1
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
	// 循环2
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
	// 第一层 左上角
	if (y == 0 && x == 1) {
		as.pass1 = 1;
		direction = DOWN;
	}
	// 第一层其他
	if (y == 0 && (x == 2 || x == 3)) {
		direction = LEFT;
	}
	// 最下层 && 右下角
	if (y == 2 && x == 3) {
		direction = UP;
	}
	// 最下层
	if (y == 2 && (x == 1 || x == 2)) {
		direction = RIGHT;
	}
	// 中间层
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
	ULONG64 mapOffset = rw.ReadLong(gd.personPtr + 地图偏移);
	if (mapOffset == 0) {
		return;
	}

	ULONG64 roomData = rw.ReadLong(rw.ReadLong(rw.ReadLong(房间编号) + 时间基址) + 顺图偏移);
	ULONG64	coordinateStructure = roomData + direction * 方向ID + 坐标顺图;
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
	// 防止已过图执行无效模拟
	if(jd.CoordinateEqual(coo, jd.GetCurrentRoom())){
		sl.GoDestation(int(startX + endX / 2), startY, jd.GetCurrentRoom(), true);
	}
}
