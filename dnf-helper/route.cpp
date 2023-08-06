#include "route.h"
#include "judge.h"
#include "readwrite.h"

MapData mapData;

int MapData::GetMapData()
{
    vector<INT> channels;
    CoordinateStruct current = jd.GetCurrentRoom();
    CoordinateStruct boss = jd.GetBossRoom();
    INT64 roomData = rw.ReadLong(rw.ReadLong(rw.ReadLong(������) + ʱ���ַ) + ����ƫ��);
    INT roomIndex = rw.ReadInt(roomData + ����ƫ��);
    INT width = rw.ReadInt(rw.ReadLong(roomData + ���ƫ��) + (INT64)roomIndex * 8 + 0);
    INT height = rw.ReadInt(rw.ReadLong(roomData + ���ƫ��) + (INT64)roomIndex * 8 + 4);
    INT64 tmp = rw.ReadLong(rw.ReadLong(roomData + ����ƫ��) + 32 * (INT64)roomIndex + 8);
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
    BOOL ���;
    BOOL �ұ�;
    BOOL �ϱ�;
    BOOL �±�;
    Ѱ·ֵ ��������[100][100];


    INT z = 0;
    BOOL �����յ� = FALSE;
    INT �ҵ�����;
    BOOL �������;
    INT λ��;
    vector<INT> ��ʱ���X����;
    vector<INT> ��ʱ���Y����;
    INT64 ��һ����;
    for (INT y = 0; y < height; y++)
    {
        for (INT x = 0; x < width; x++)
        {
            if (channels.at(i) != 0)
            {
                ��������[x][y].x = x;
                ��������[x][y].y = y;
                ��������[x][y].��ɫ = (INT)RGB(255, 255, 255);
            }
            else
            {
                ��������[x][y].x = x;
                ��������[x][y].y = y;
                ��������[x][y].��ɫ = (INT)RGB(0, 0, 0);
            }
            if (current.x == x && current.y == y)
            {
                ��������[x][y].��ɫ = (INT)RGB(255, 255, 0);
                ��������[x][y].��ֵ = 0;
                ��������[x][y].��� = TRUE;
            }
            if (boss.x == x && boss.y == y)
            {
                ��������[x][y].��ɫ = (INT)RGB(255, 255, 255);
                ��������[x][y].�յ� = TRUE;
            }
            ��� = JudgeDirection(channels.at(i), 0);
            �ұ� = JudgeDirection(channels.at(i), 1);
            �ϱ� = JudgeDirection(channels.at(i), 2);
            �±� = JudgeDirection(channels.at(i), 3);
            if (��� == TRUE)
            {
                ��������[x][y].�� = TRUE;
            }
            if (�ұ� == TRUE)
            {
                ��������[x][y].�� = TRUE;
            }
            if (�ϱ� == TRUE)
            {
                ��������[x][y].�� = TRUE;
            }
            if (�±� == TRUE)
            {
                ��������[x][y].�� = TRUE;
            }
            i = i + 1;
        }
    }

    while (�����յ� == FALSE)
    {
        for (INT y = 0; y < height; y++)
        {
            for (INT x = 0; x < width; x++)
            {
                �ҵ����� = ��������[x][y].��ֵ;
                if (�ҵ����� == z && ��������[x][y].��� == TRUE)
                {
                    ��������[x][y].��� = FALSE;
                    if (x - 1 != -1)
                    {
                        if (��������[x][y].�� == TRUE && ��������[x - 1][y].��ɫ == (INT)RGB(255, 255, 255))
                        {
                            ��������[x - 1][y].��ֵ = z + 1;
                            ��������[x - 1][y].��ɫ = (INT)RGB(255, 255, 0);
                            ��������[x - 1][y].��� = TRUE;
                        }
                        if (��������[x][y].�� == TRUE && boss.x == x - 1 && boss.y == y)
                        {
                            �����յ� = TRUE;
                            break;
                        }
                    }
                    if (x + 1 <= width - 1)
                    {
                        if (��������[x][y].�� == TRUE && ��������[x + 1][y].��ɫ == (INT)RGB(255, 255, 255))
                        {
                            ��������[x + 1][y].��ֵ = z + 1;
                            ��������[x + 1][y].��ɫ = (INT)RGB(255, 255, 0);
                            ��������[x + 1][y].��� = TRUE;
                        }
                        if (��������[x][y].�� == TRUE && boss.x == x + 1 && boss.y == y)
                        {
                            �����յ� = TRUE;
                            break;
                        }
                    }
                    if (y - 1 != -1)
                    {
                        if (��������[x][y].�� == TRUE && ��������[x][y - 1].��ɫ == (INT)RGB(255, 255, 255))
                        {
                            ��������[x][y - 1].��ֵ = z + 1;
                            ��������[x][y - 1].��ɫ = (INT)RGB(255, 255, 0);
                            ��������[x][y - 1].��� = TRUE;
                        }
                        if (��������[x][y].�� == TRUE && boss.x == x && boss.y == y - 1)
                        {
                            �����յ� = TRUE;
                            break;
                        }
                    }
                    if (y + 1 <= height - 1)
                    {
                        if (��������[x][y].�� == TRUE && ��������[x][y + 1].��ɫ == (INT)RGB(255, 255, 255))
                        {
                            ��������[x][y + 1].��ֵ = z + 1;
                            ��������[x][y + 1].��ɫ = (INT)RGB(255, 255, 0);
                            ��������[x][y + 1].��� = TRUE;
                        }
                        if (��������[x][y].�� == TRUE && boss.x == x && boss.y == y + 1)
                        {
                            �����յ� = TRUE;
                            break;
                        }
                    }
                }
            }
        }
        z = z + 1;
    }
    ��������[boss.x][boss.y].��ֵ = z;
    ��ʱ���X����.push_back(boss.x);
    ��ʱ���Y����.push_back(boss.y);
    λ�� = ��������[boss.x][boss.y].��ֵ;
    if (�����յ�)
    {
        for (INT y = 0; y < height; y++)
        {

            for (INT x = 0; x < width; x++)
            {
                if (��������[x][y].�յ� == TRUE)
                {
                    ������� = FALSE;
                    if (x - 1 != -1)
                    {
                        if (��������[x][y].�� == TRUE && ��������[x - 1][y].��ɫ == (INT)RGB(255, 255, 0))
                        {
                            if (��������[x - 1][y].��ֵ == λ�� - 1)
                            {
                                λ�� = ��������[x - 1][y].��ֵ;
                            }
                        }
                    }
                    if (x + 1 <= width - 1)
                    {
                        if (��������[x][y].�� == TRUE && ��������[x + 1][y].��ɫ == (INT)RGB(255, 255, 0))
                        {
                            if (��������[x + 1][y].��ֵ == λ�� - 1)
                            {
                                λ�� = ��������[x + 1][y].��ֵ;
                            }
                        }
                    }
                    if (y - 1 != -1)
                    {
                        if (��������[x][y].�� == TRUE && ��������[x][y - 1].��ɫ == (INT)RGB(255, 255, 0))
                        {
                            if (��������[x][y - 1].��ֵ == λ�� - 1)
                            {
                                λ�� = ��������[x][y - 1].��ֵ;
                            }
                        }
                    }
                    if (y + 1 <= height - 1)
                    {
                        if (��������[x][y].�� == TRUE && ��������[x][y + 1].��ɫ == (INT)RGB(255, 255, 0))
                        {
                            if (��������[x][y + 1].��ֵ == λ�� - 1)
                            {
                                λ�� = ��������[x][y + 1].��ֵ;
                            }
                        }
                    }
                    while (������� == FALSE)
                    {
                        ������� = TRUE;
                        if (x - 1 != -1)
                        {
                            if (��������[x][y].�� == TRUE && ��������[x - 1][y].��ֵ == λ��)
                            {
                                ��������[x - 1][y].��ɫ = (INT)RGB(0, 0, 255);
                                ��ʱ���X����.push_back(��������[x - 1][y].x);
                                ��ʱ���Y����.push_back(��������[x - 1][y].y);
                                ������� = FALSE;
                                λ�� = λ�� - 1;
                                x = x - 1;
                                continue;
                            }
                        }
                        if (x + 1 <= width - 1)
                        {
                            if (��������[x][y].�� == TRUE && ��������[x + 1][y].��ֵ == λ��)
                            {
                                ��������[x + 1][y].��ɫ = (INT)RGB(0, 0, 255);
                                ��ʱ���X����.push_back(��������[x + 1][y].x);
                                ��ʱ���Y����.push_back(��������[x + 1][y].y);
                                ������� = FALSE;
                                λ�� = λ�� - 1;
                                x = x + 1;
                                continue;
                            }
                        }
                        if (y - 1 != -1)
                        {
                            if (��������[x][y].�� == TRUE && ��������[x][y - 1].��ֵ == λ��)
                            {
                                ��������[x][y - 1].��ɫ = (INT)RGB(0, 0, 255);
                                ��ʱ���X����.push_back(��������[x][y - 1].x);
                                ��ʱ���Y����.push_back(��������[x][y - 1].y);
                                ������� = FALSE;
                                λ�� = λ�� - 1;
                                y = y - 1;
                                continue;
                            }
                        }
                        if (y + 1 <= height - 1)
                        {
                            if (��������[x][y].�� == TRUE && ��������[x][y + 1].��ֵ == λ��)
                            {
                                ��������[x][y + 1].��ɫ = (INT)RGB(0, 0, 255);
                                ��ʱ���X����.push_back(��������[x][y + 1].x);
                                ��ʱ���Y����.push_back(��������[x][y + 1].y);
                                ������� = FALSE;
                                λ�� = λ�� - 1;
                                y = y + 1;
                                continue;
                            }
                        }
                    }
                }
            }
        }
    }
    ��һ���� = (INT64)��ʱ���X����.size();
    if (current.x < ��ʱ���X����.at(��һ���� - 2) && current.y == ��ʱ���Y����.at(��һ���� - 2))
    {
        return (1); //right
    }
    if (current.x > ��ʱ���X����.at(��һ���� - 2) && current.y == ��ʱ���Y����.at(��һ���� - 2))
    {
        return (0);// left
    }
    if (current.x == ��ʱ���X����.at(��һ���� - 2) && current.y < ��ʱ���Y����.at(��һ���� - 2))
    {
        return (3); // ��
    }
    if (current.x == ��ʱ���X����.at(��һ���� - 2) && current.y > ��ʱ���Y����.at(��һ���� - 2))
    {
        return (2); // ��
    }
    return (-1);


}