#pragma once
#include "common.h"
struct TaskStruct {
	// 任务名称
	wstring taskName;
	// 任务条件
	wstring taskConditional;
	// 任务id
	int taskId;
};

struct TaskSkip {
	// 是否能跳过
	bool isCanSkip;
	// 任务等级
	int taskLevel;
};


class Task {
public:
	// 执行任务
	static void HandleMain();
	// 主线任务
	static TaskStruct MainLineTask();
	// 任务能否跳过
	static TaskSkip CouldSkip(int taskId);
	// 任务条件
	static int Conditional(wstring contiditional);
	// 任务地图
	static int TaskMap(int taskId);
	// 提交任务
	static void SubmitTask();
	// 完成状态 -1 任务未接  0 任务完成 1 已接任务
	static int FinishStatus(int taskId);
	// 取最高等级地图
	static int HighestMap();




};