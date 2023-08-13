#pragma once

#include "common.h"

class Simulate {

public:
	static void GoDestation(int targetX, int targetY, CoordinateStruct beforeCoordinate = {-1, -1, -1}, bool findGoods = false);

};

extern Simulate sl;