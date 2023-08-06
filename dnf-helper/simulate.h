#pragma once

#include "common.h"

class Simulate {

public:
	static void GoDestation(int targetX, int targetY);

	static void OverMap(int targetX, int targetY, CoordinateStruct beforeCoordinate);

};

extern Simulate sl;