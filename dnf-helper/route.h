#pragma once
#include "common.h"

struct 寻路值 {
    int x = 0;
    int y = 0;
    int 颜色 = 0;
    int 数值 = 0;
    bool 终点 = FALSE;
    bool 起点 = FALSE;
    bool 上 = FALSE;
    bool 下 = FALSE;
    bool 左 = FALSE;
    bool 右 = FALSE;
};

class MapData {

public:
    // 获取地图数据
    static int GetMapData();
    // 生成地图
    static int GenerateMap(CoordinateStruct& current, CoordinateStruct& boss, int width, int height, vector<INT>& channels);
    // 判断方向
    static bool JudgeDirection(int channel, int direction);

};


extern MapData mapData;