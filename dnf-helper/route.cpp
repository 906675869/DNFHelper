#include "route.h"
#include "judge.h"
#include "readwrite.h"

MapData mapData;

int MapData::GetMapData()
{
    vector<INT> channels;
    CoordinateStruct current = jd.GetCurrentRoom();
    CoordinateStruct boss = jd.GetBossRoom();
    INT64 roomData = rw.ReadLong(rw.ReadLong(rw.ReadLong(房间编号) + 时间基址) + 门型偏移);
    INT roomIndex = rw.ReadInt(roomData + 索引偏移);
    INT width = rw.ReadInt(rw.ReadLong(roomData + 宽高偏移) + (INT64)roomIndex * 8 + 0);
    INT height = rw.ReadInt(rw.ReadLong(roomData + 宽高偏移) + (INT64)roomIndex * 8 + 4);
    INT64 tmp = rw.ReadLong(rw.ReadLong(roomData + 数组偏移) + 32 * (INT64)roomIndex + 8);
    INT channelNum = width * height;
    for (INT i = 0; i < channelNum; i++)
    {
        channels.push_back(rw.ReadInt(tmp + (INT64)i * 4));
    }
    return (GenerateMap(current, boss, width, height, channels));
}

bool MapData::JudgeDirection(int channel, int direction) {
    unsigned char directions[4];
    unsigned char directionCollection[16][4] = { { 0, 0, 0, 0 },{ 0, 1, 0, 0 },{ 0, 0, 1, 0 },{ 0, 1, 1, 0 },{ 1, 0, 0, 0 },{ 1, 1, 0, 0 },{ 1, 0, 1, 0 },{ 1, 1, 1, 0 },
    { 0, 0, 0, 1 },{ 0, 1, 0, 1 },{ 0, 0, 1, 1 },{ 0, 1, 1, 1 },{ 1, 0, 0, 1 },{ 1, 1, 0, 1 },{ 1, 0, 1, 1 },{ 1, 1, 1, 1 } };
    if (channel >= 0 && channel <= 15)
    {
        for (int i = 0; i < 4; i++)
        {
            directions[i] = directionCollection[channel][i];
        }
    }
    else
    {
        for (int i = 0; i < 4; i++)
        {
            directions[i] = 0;
        }
    }

    if (directions[direction] == 1)
    {
        return true;
    }
    else
    {
        return false;
    }

}


int  MapData::GenerateMap(CoordinateStruct& current, CoordinateStruct& boss, int width, int height, vector<INT>& channels) {
    INT i = 0;
    BOOL 左边;
    BOOL 右边;
    BOOL 上边;
    BOOL 下边;
    寻路值 坐标数组[100][100];


    INT z = 0;
    BOOL 到达终点 = FALSE;
    INT 找到标题;
    BOOL 到达起点;
    INT 位置;
    vector<INT> 临时存放X坐标;
    vector<INT> 临时存放Y坐标;
    INT64 下一坐标;
    for (INT y = 0; y < height; y++)
    {
        for (INT x = 0; x < width; x++)
        {
            if (channels.at(i) != 0)
            {
                坐标数组[x][y].x = x;
                坐标数组[x][y].y = y;
                坐标数组[x][y].颜色 = (INT)RGB(255, 255, 255);
            }
            else
            {
                坐标数组[x][y].x = x;
                坐标数组[x][y].y = y;
                坐标数组[x][y].颜色 = (INT)RGB(0, 0, 0);
            }
            if (current.x == x && current.y == y)
            {
                坐标数组[x][y].颜色 = (INT)RGB(255, 255, 0);
                坐标数组[x][y].数值 = 0;
                坐标数组[x][y].起点 = TRUE;
            }
            if (boss.x == x && boss.y == y)
            {
                坐标数组[x][y].颜色 = (INT)RGB(255, 255, 255);
                坐标数组[x][y].终点 = TRUE;
            }
            左边 = JudgeDirection(channels.at(i), 0);
            右边 = JudgeDirection(channels.at(i), 1);
            上边 = JudgeDirection(channels.at(i), 2);
            下边 = JudgeDirection(channels.at(i), 3);
            if (左边 == TRUE)
            {
                坐标数组[x][y].左 = TRUE;
            }
            if (右边 == TRUE)
            {
                坐标数组[x][y].右 = TRUE;
            }
            if (上边 == TRUE)
            {
                坐标数组[x][y].上 = TRUE;
            }
            if (下边 == TRUE)
            {
                坐标数组[x][y].下 = TRUE;
            }
            i = i + 1;
        }
    }

    while (到达终点 == FALSE)
    {
        for (INT y = 0; y < height; y++)
        {
            for (INT x = 0; x < width; x++)
            {
                找到标题 = 坐标数组[x][y].数值;
                if (找到标题 == z && 坐标数组[x][y].起点 == TRUE)
                {
                    坐标数组[x][y].起点 = FALSE;
                    if (x - 1 != -1)
                    {
                        if (坐标数组[x][y].左 == TRUE && 坐标数组[x - 1][y].颜色 == (INT)RGB(255, 255, 255))
                        {
                            坐标数组[x - 1][y].数值 = z + 1;
                            坐标数组[x - 1][y].颜色 = (INT)RGB(255, 255, 0);
                            坐标数组[x - 1][y].起点 = TRUE;
                        }
                        if (坐标数组[x][y].左 == TRUE && boss.x == x - 1 && boss.y == y)
                        {
                            到达终点 = TRUE;
                            break;
                        }
                    }
                    if (x + 1 <= width - 1)
                    {
                        if (坐标数组[x][y].右 == TRUE && 坐标数组[x + 1][y].颜色 == (INT)RGB(255, 255, 255))
                        {
                            坐标数组[x + 1][y].数值 = z + 1;
                            坐标数组[x + 1][y].颜色 = (INT)RGB(255, 255, 0);
                            坐标数组[x + 1][y].起点 = TRUE;
                        }
                        if (坐标数组[x][y].右 == TRUE && boss.x == x + 1 && boss.y == y)
                        {
                            到达终点 = TRUE;
                            break;
                        }
                    }
                    if (y - 1 != -1)
                    {
                        if (坐标数组[x][y].上 == TRUE && 坐标数组[x][y - 1].颜色 == (INT)RGB(255, 255, 255))
                        {
                            坐标数组[x][y - 1].数值 = z + 1;
                            坐标数组[x][y - 1].颜色 = (INT)RGB(255, 255, 0);
                            坐标数组[x][y - 1].起点 = TRUE;
                        }
                        if (坐标数组[x][y].上 == TRUE && boss.x == x && boss.y == y - 1)
                        {
                            到达终点 = TRUE;
                            break;
                        }
                    }
                    if (y + 1 <= height - 1)
                    {
                        if (坐标数组[x][y].下 == TRUE && 坐标数组[x][y + 1].颜色 == (INT)RGB(255, 255, 255))
                        {
                            坐标数组[x][y + 1].数值 = z + 1;
                            坐标数组[x][y + 1].颜色 = (INT)RGB(255, 255, 0);
                            坐标数组[x][y + 1].起点 = TRUE;
                        }
                        if (坐标数组[x][y].下 == TRUE && boss.x == x && boss.y == y + 1)
                        {
                            到达终点 = TRUE;
                            break;
                        }
                    }
                }
            }
        }
        z = z + 1;
    }
    坐标数组[boss.x][boss.y].数值 = z;
    临时存放X坐标.push_back(boss.x);
    临时存放Y坐标.push_back(boss.y);
    位置 = 坐标数组[boss.x][boss.y].数值;
    if (到达终点)
    {
        for (INT y = 0; y < height; y++)
        {

            for (INT x = 0; x < width; x++)
            {
                if (坐标数组[x][y].终点 == TRUE)
                {
                    到达起点 = FALSE;
                    if (x - 1 != -1)
                    {
                        if (坐标数组[x][y].左 == TRUE && 坐标数组[x - 1][y].颜色 == (INT)RGB(255, 255, 0))
                        {
                            if (坐标数组[x - 1][y].数值 == 位置 - 1)
                            {
                                位置 = 坐标数组[x - 1][y].数值;
                            }
                        }
                    }
                    if (x + 1 <= width - 1)
                    {
                        if (坐标数组[x][y].右 == TRUE && 坐标数组[x + 1][y].颜色 == (INT)RGB(255, 255, 0))
                        {
                            if (坐标数组[x + 1][y].数值 == 位置 - 1)
                            {
                                位置 = 坐标数组[x + 1][y].数值;
                            }
                        }
                    }
                    if (y - 1 != -1)
                    {
                        if (坐标数组[x][y].上 == TRUE && 坐标数组[x][y - 1].颜色 == (INT)RGB(255, 255, 0))
                        {
                            if (坐标数组[x][y - 1].数值 == 位置 - 1)
                            {
                                位置 = 坐标数组[x][y - 1].数值;
                            }
                        }
                    }
                    if (y + 1 <= height - 1)
                    {
                        if (坐标数组[x][y].下 == TRUE && 坐标数组[x][y + 1].颜色 == (INT)RGB(255, 255, 0))
                        {
                            if (坐标数组[x][y + 1].数值 == 位置 - 1)
                            {
                                位置 = 坐标数组[x][y + 1].数值;
                            }
                        }
                    }
                    while (到达起点 == FALSE)
                    {
                        到达起点 = TRUE;
                        if (x - 1 != -1)
                        {
                            if (坐标数组[x][y].左 == TRUE && 坐标数组[x - 1][y].数值 == 位置)
                            {
                                坐标数组[x - 1][y].颜色 = (INT)RGB(0, 0, 255);
                                临时存放X坐标.push_back(坐标数组[x - 1][y].x);
                                临时存放Y坐标.push_back(坐标数组[x - 1][y].y);
                                到达起点 = FALSE;
                                位置 = 位置 - 1;
                                x = x - 1;
                                continue;
                            }
                        }
                        if (x + 1 <= width - 1)
                        {
                            if (坐标数组[x][y].右 == TRUE && 坐标数组[x + 1][y].数值 == 位置)
                            {
                                坐标数组[x + 1][y].颜色 = (INT)RGB(0, 0, 255);
                                临时存放X坐标.push_back(坐标数组[x + 1][y].x);
                                临时存放Y坐标.push_back(坐标数组[x + 1][y].y);
                                到达起点 = FALSE;
                                位置 = 位置 - 1;
                                x = x + 1;
                                continue;
                            }
                        }
                        if (y - 1 != -1)
                        {
                            if (坐标数组[x][y].上 == TRUE && 坐标数组[x][y - 1].数值 == 位置)
                            {
                                坐标数组[x][y - 1].颜色 = (INT)RGB(0, 0, 255);
                                临时存放X坐标.push_back(坐标数组[x][y - 1].x);
                                临时存放Y坐标.push_back(坐标数组[x][y - 1].y);
                                到达起点 = FALSE;
                                位置 = 位置 - 1;
                                y = y - 1;
                                continue;
                            }
                        }
                        if (y + 1 <= height - 1)
                        {
                            if (坐标数组[x][y].下 == TRUE && 坐标数组[x][y + 1].数值 == 位置)
                            {
                                坐标数组[x][y + 1].颜色 = (INT)RGB(0, 0, 255);
                                临时存放X坐标.push_back(坐标数组[x][y + 1].x);
                                临时存放Y坐标.push_back(坐标数组[x][y + 1].y);
                                到达起点 = FALSE;
                                位置 = 位置 - 1;
                                y = y + 1;
                                continue;
                            }
                        }
                    }
                }
            }
        }
    }
    下一坐标 = (INT64)临时存放X坐标.size();
    if (current.x < 临时存放X坐标.at(下一坐标 - 2) && current.y == 临时存放Y坐标.at(下一坐标 - 2))
    {
        return (1); //right
    }
    if (current.x > 临时存放X坐标.at(下一坐标 - 2) && current.y == 临时存放Y坐标.at(下一坐标 - 2))
    {
        return (0);// left
    }
    if (current.x == 临时存放X坐标.at(下一坐标 - 2) && current.y < 临时存放Y坐标.at(下一坐标 - 2))
    {
        return (3); // 下
    }
    if (current.x == 临时存放X坐标.at(下一坐标 - 2) && current.y > 临时存放Y坐标.at(下一坐标 - 2))
    {
        return (2); // 上
    }
    return (-1);


}