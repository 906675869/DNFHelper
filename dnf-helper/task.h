#pragma once
#include "common.h"
struct TaskStruct {
	// ��������
	wstring taskName;
	// ��������
	wstring taskConditional;
	// ����id
	int taskId;
};

struct TaskSkip {
	// �Ƿ�������
	bool isCanSkip;
	// ����ȼ�
	int taskLevel;
};


class Task {
public:
	// ִ������
	static int HandleMain();
	// ��������
	static TaskStruct MainLineTask();
	// �����ܷ�����
	static TaskSkip CouldSkip(int taskId);
	// ��������
	static int Conditional(string contiditional);
	// �����ͼ
	static int TaskMap(int taskId);
	// �ύ����
	static void SubmitTask();
	// ���״̬ -1 ����δ��  0 ������� 1 �ѽ�����
	static int FinishStatus(int taskId);
	// ȡ��ߵȼ���ͼ
	static int HighestMap();

};

extern Task tk;