#pragma once
#include "common.h"

struct Ѱ·ֵ {
    int x = 0;
    int y = 0;
    int ��ɫ = 0;
    int ��ֵ = 0;
    bool �յ� = FALSE;
    bool ��� = FALSE;
    bool �� = FALSE;
    bool �� = FALSE;
    bool �� = FALSE;
    bool �� = FALSE;
};

class MapData {

public:
    // ��ȡ��ͼ����
    static int GetMapData();
    // ���ɵ�ͼ
    static int GenerateMap(CoordinateStruct& current, CoordinateStruct& boss, int width, int height, vector<INT>& channels);
    // �жϷ���
    static bool JudgeDirection(int channel, int direction);

};


extern MapData mapData;