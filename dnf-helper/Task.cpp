#include "task.h"
#include "judge.h"
#include <algorithm>
#include "call.h"
#include "helper.h"
#include <iostream>
#include "pack.h"
using namespace std;
static bool refreshTask = FALSE;

int Task::HandleMain()
{
	SubmitTask();
	int mapId = 0, nextTaskId = 0;
	while (gd.autoSwitch) {
		Sleep(300);
		TaskStruct task = MainLineTask();
		if (task.taskId != nextTaskId && task.taskId > 0) {
			nextTaskId = task.taskId;
			cout << "主线任务-> 任务名称" << task.taskName.c_str() <<endl;
			cout << "主线任务-> 任务条件" << task.taskConditional.c_str() <<endl;
			cout << "主线任务-> 任务Id" << task.taskId <<endl;
		}
		// 无主线任务
		if (task.taskId == 0) {
			if (!refreshTask) {
				cout << "暂时无任务或者卡任务" << endl;
				pk.RoleList();
				Sleep(200);
				pk.ChooseRole(as.chooseRoleNum);
				Sleep(500);
				refreshTask = TRUE;
				continue;
			}
			else {
				cout << "无法完成任务，执行适应等级地图" << endl;
				return HighestMap();
			}
		}
		refreshTask = FALSE;
		if (task.taskId == 3509 || task.taskId == 5943) {
			cout << "无法完成任务，执行适应等级地图" << endl;
			return HighestMap();
		}
		TaskSkip taskSkip = CouldSkip(task.taskId);
		if (taskSkip.isCanSkip && taskSkip.taskLevel != 85 && taskSkip.taskLevel != 86) {
			cl.SkipMissionCall();
		}
		if (FinishStatus(task.taskId) == -1) {
			cl.AcceptMissionCall(task.taskId);
		}
		// todo 
		string taskCoditional = wstring2string(task.taskConditional);
		if (Conditional(taskCoditional) == 1) {
			cl.CompleteMissionCall(task.taskId);
		}
		if (Conditional(taskCoditional) == 2) {
			mapId = TaskMap(task.taskId);
			if (mapId > 0) {
				return mapId;
			}
		}
		if (Conditional(taskCoditional) == 3) {
			mapId = HighestMap();
			cout << "材料任务无法完成，执行最高等级地图" << endl;
			return mapId;
		}
		return mapId;

	}
}

TaskStruct Task::MainLineTask()
{
	ULONG64 task_addr = rw.ReadLong(任务基址);
	ULONG64	start = rw.ReadLong(task_addr + 全部任务首地址);
	ULONG64 end = rw.ReadLong(task_addr + 全部任务尾地址);
	int	num = int((end - start) / 8);
	for (int i = 0; i < num; i++) {
		ULONG64 task_ptr = rw.ReadLong(start + i * 8);
		int	task_type = rw.ReadInt(task_ptr + 任务类型);
		if (task_type == 0) {
			int task_length = rw.ReadInt(task_ptr + 任务大小);
			wstring task_name;
			if (task_length > 7) {
				ULONG64 tmp = rw.ReadLong(task_ptr + 16);
				task_name = UnicodeToAnsi(rw.ReadBytes(tmp, 100));
			}
			else {
				task_name = UnicodeToAnsi(rw.ReadBytes(task_ptr + 16, 100));
			}
			wstring task_conditional = UnicodeToAnsi(rw.ReadBytes(rw.ReadLong(task_ptr + 任务条件), 100));
				// # 任务编号
			int	task_id = rw.ReadInt(task_ptr);
			return { task_name , task_conditional , task_id };
		}
		
	}

	return TaskStruct();
}

TaskSkip Task::CouldSkip(int taskId)
{
	ULONG64 task_addr = rw.ReadLong(任务基址);
	ULONG64 start = rw.ReadLong(task_addr + 已接任务首地址);
	ULONG64 end = rw.ReadLong(task_addr + 已接任务尾地址);
	int	num = int((end - start) / 16);

	for (int i = 0; i < num; i++) {
		ULONG64 task_ptr = rw.ReadLong(start + i * 16);
		if (rw.ReadInt(task_ptr) == taskId){
			int task_level = rw.ReadInt(task_ptr + 任务等级);
			if (task_level < jd.GetRoleLevel()) {
				return TaskSkip{ TRUE, task_level };
			}
		}
	}
	return TaskSkip{ FALSE, 0 };
}

int Task::Conditional(string contiditional)
{
	string brush_conditions = "[meet npc]#[seek n meet npc]#[reach the range]#[look cinematic]#[question]#[quest clear]";
	auto result = std::search(brush_conditions.begin(), brush_conditions.end(), contiditional.begin(), contiditional.end());
	if (result != brush_conditions.end()) {
		return 1;
	}
	brush_conditions = "[hunt monster]#[hunt enemy]#[condition under clear]#[clear map]#[question]#[seeking]#[clear dungeon index]";
	auto result = std::search(brush_conditions.begin(), brush_conditions.end(), contiditional.begin(), contiditional.end());
	if (result != brush_conditions.end()) {
		return 2;
	}
	return 0;
}

int Task::TaskMap(int taskId)
{
	ULONG64 task_addr = rw.ReadLong(任务基址);
	ULONG64 start = rw.ReadLong(task_addr + 已接任务首地址);
	ULONG64 end = rw.ReadLong(task_addr + 已接任务尾地址);
	int	num = int((end - start) / 16);

	for (int i = 0; i < num; i++) {
		ULONG64 task_ptr = rw.ReadLong(start + i * 16);
		if (rw.ReadInt(task_ptr) == taskId) {
			ULONG64 task_data = rw.ReadLong(task_ptr + 任务副本);
			return rw.ReadInt(task_data);
		}
	}
	return 0;
}

void Task::SubmitTask()
{
	ULONG64 task_addr = rw.ReadLong(任务基址);
	ULONG64	start = rw.ReadLong(task_addr + 全部任务首地址);
	ULONG64 end = rw.ReadLong(task_addr + 全部任务尾地址);
	int	num = int((end - start) / 8);

	for (int i = 0; i < num; i++) {
		ULONG64 task_ptr = rw.ReadLong(start + i * 8);
		int	task_type = rw.ReadInt(task_ptr + 任务类型);
		if (task_type == 0) {
			int task_id = rw.ReadInt(task_ptr);
			cl.SubmitMissionCall(task_id);
		}
	}

	start = rw.ReadLong(task_addr + 已接任务首地址);
	end = rw.ReadLong(task_addr + 已接任务尾地址);
	num = int((end - start) / 16);
	for (int i = 0; i < num; i++) {
		int task_ptr = rw.ReadLong(start + i * 16);
		int	task_type = rw.ReadInt(task_ptr + 任务类型);
		if(task_type == 0){
			int task_id = rw.ReadInt(task_ptr);
			cl.SubmitMissionCall(task_id);
		}
	}
}

int Task::FinishStatus(int taskId)
{
	ULONG64 task_addr = rw.ReadLong(任务基址);
	ULONG64	start = rw.ReadLong(task_addr + 已接任务首地址);
	ULONG64 end = rw.ReadLong(task_addr + 已接任务尾地址);
	int	num = int((end - start) / 16);

	int tmp_arr[3];

	for (int i = 0; i < num; i++) {
		ULONG64 pointer = rw.ReadLong(start + i * 16);
		if (rw.ReadInt(pointer) == taskId) {
			int frequency = rw.ReadInt(start + i * 16 + 8);
			if (frequency < 512) {
				return frequency;
			}
			else if (frequency == 512) {
				return 1;
			}
			tmp_arr[0] = int(frequency % 512);
			int	the_rest = int(frequency) - tmp_arr[0];
			if (the_rest < 262144) {
				tmp_arr[1] = int(the_rest / 512);// 无用？
				tmp_arr[1] = int(the_rest % 262144 / 512);
			}
			the_rest = int(the_rest - tmp_arr[0] * 512);
			if (the_rest < 262144) {
				tmp_arr[2] = 0;
				tmp_arr[2] = int(the_rest % 262144);
			}
			sort(tmp_arr, tmp_arr + 3 ,greater<int>());
			if (tmp_arr[0] == 0) {
				tmp_arr[0] = 1;
			}
			return tmp_arr[0];
		}
	}
	return -1;
}








int Task::HighestMap()
{
	int role_level = jd.GetRoleLevel();
	if (role_level <= 17) {
		if (role_level <= 3) {
			return 3; // 雷鸣废墟
		}
		if (role_level <= 5) {
			return 5; // 雷鸣废墟
		}
		if (role_level <= 8) {
			return 6; // 猛毒雷鸣废墟
		}
		if (role_level <= 11) {
			return 9; // 冰霜幽暗密林
		}
		if (role_level <= 13) {
			return 7; // 格拉卡
		}
		if (role_level <= 15) {
			return 8; // 烈焰格拉卡
		}if (role_level <= 17) {
			return 1000; // 暗黑雷鸣废墟
		}return 0;
	}

	if (role_level <= 23) {
		if (role_level <= 18) {
			return 1000; // 10 龙人之塔
		}if (role_level <= 19) {
			return 12; // 人偶玄关
		}if (role_level <= 20) {
			return 13; // 石巨人塔
		}if (role_level <= 21) {
			return 14; // 黑暗玄廊
		}if (role_level <= 22) {
			return 17; // 悬空城
		}if (role_level <= 23) {
			return 15; // 城主宫殿
		}return 0

			; // 神殿脊椎
	}

	if (role_level <= 29) {
		if (role_level <= 24) {
			return 15; // 21 神殿外围
		}
		if (role_level <= 25) {
			return 22; // 树精丛林
		}
		if (role_level <= 26) {
			return 23; // 炼狱
		}
		if (role_level <= 27) {
			return 24; // 极昼
		}
		if (role_level <= 28) {
			return 25; // 第一脊椎
		}
		if (role_level <= 29) {
			return 26; // 第二脊椎
		}
		return 0;

	}
	//; // 暗精灵地区
	if (role_level <= 35) {
		if (role_level <= 30) {
			return 26; // 31  浅栖之地
		}
		if (role_level <= 31) {
			return 32; // 蜘蛛洞穴
		}
		if (role_level <= 32) {
			return 150; // 蜘蛛王国
		}
		if (role_level <= 33) {
			return 151; // 英雄冢
		}
		if (role_level <= 34) {
			return 35; // 暗精灵墓地
		}
		if (role_level <= 35) {
			return 34; // 熔岩穴
		}
		return 0;
		//; // 祭坛
	}
	if (role_level <= 39) {
		if (role_level <= 36) {
			return 34; // 152 暴君的祭坛
		}
		if (role_level <= 37) {
			return 153; // 黄金矿洞
		}
		if (role_level <= 38) {
			return 154; // 远古墓穴深处
		}
		if (role_level <= 39) {
			return 154; // 远古墓穴深处
		}
		return 0;
		// 雪山
	}

	if (role_level <= 45) {
		if (role_level <= 40) {
			return 154; // 40 山脊
		}if (role_level <= 41) {
			return 41; // 冰心少年
		}if (role_level <= 42) {
			return 42; // 利库天井
		}if (role_level <= 44) {
			return 141; // 布万加的修炼场
		}if (role_level <= 45) {
			return 141; // 布万加的修炼场
		}
		return 0;

		//; // 绿都
	}

	if (role_level <= 49) {
		if (role_level <= 46) {
			return 141; // 61  绿都格罗兹尼
		}
		if (role_level <= 47) {
			return 50; // 堕落的盗贼
		}
		if (role_level <= 48) {
			return 51; // 迷乱之村哈穆林
		}
		if (role_level <= 49) {
			return 53; // 疑惑之村
		}
		return 0;
	}

	if (role_level <= 53) {
		if (role_level <= 50) {
			return 53; // 144  炽晶森林
		}
		if (role_level <= 51) {
			return 145; // 冰晶森林
		}
		if (role_level <= 52) {
			return 146; // 水晶矿脉
		}
		if (role_level <= 53) {
			return 148; // 幽冥监狱
		}
		return 0;
	}

	if (role_level <= 58) {
		if (role_level <= 54) {
			return 148; // 156 蘑菇庄园
		}
		if (role_level <= 55) {
			return 157; // 蚁后的巢穴
		}
		if (role_level <= 56) {
			return 158; // 腐烂之地
		}
		if (role_level <= 57) {
			return 159; // 赫顿玛尔旧街区
		}
		if (role_level <= 58) {
			return 160; // 鲨鱼栖息地
		}
		return 0;
	}


	if (role_level <= 62) {
		if (role_level <= 59) {
			return 160; // 162  '人鱼国度
		}
		if (role_level <= 60) {
			return 163; // GBL女神殿
		}
		if (role_level <= 61) {
			return 164; // 树精繁殖地
		}
		if (role_level <= 62) {
			return 164; // 树精繁殖地
		}
		return 0;
	}

	if (role_level <= 70) {
		if (role_level <= 63) {
			return 164; // 80  '根特外围
		}
		if (role_level <= 64) {
			return 81; // 根特东门
		}
		if (role_level <= 65) {
			return 82; // 根特南门
		}
		if (role_level <= 66) {
			return 88; // 根特北门
		}
		if (role_level <= 67) {
			return 88; // 根特北门
		}
		if (role_level <= 68) {
			return 83; // 夜间袭击战
		}
		if (role_level <= 69) {
			return 84; // 补给阻断站
		}
		if (role_level <= 70) {
			return 85; // 追击歼灭战
		}
		return 0;
		//; // 海上列车
	}

	if (role_level <= 74) {
		if (role_level <= 71) {
			return 85; // 86 列车上的海贼
		}
		if (role_level <= 71) {
			return 87; // 夺回西部线
		}
		if (role_level <= 73) {
			return 92; // 雾都赫伊斯
		}
		if (role_level <= 74) {
			return 93; // 阿登高地
		}
		return 0;
	}

	if (role_level <= 80) {
		if (role_level <= 75) {
			return 93; // 70 格兰之火
		}
		if (role_level <= 76) {
			return 71; // 瘟疫之源
		}
		if (role_level <= 77) {
			return 72; // 卡勒特之刃
		}
		if (role_level <= 78) {
			return 74; // 绝密区域
		}
		if (role_level <= 79) {
			return 75; // 昔日悲鸣
		}
		if (role_level <= 80) {
			return 76; // 凛冬
		}
		return 0;
	}
	if (role_level <= 85) {
		if (role_level <= 81) {
			return 76; // 102 普鲁兹发电站
		}
		if (role_level <= 82) {
			return 103; // 特伦斯发电站
		}
		if (role_level <= 85) {
			return 104; // 格蓝迪发电站
		}
		return 0;
	}
	if (role_level <= 86) {
		return 192; // 钢铁之臂
	}

	if (role_level <= 90) {
		if (role_level <= 87) {
			return 310; // 时间广场
		}
		if (role_level <= 88) {
			return 312; // 恐怖的栖息地
		}
		if (role_level <= 89) {
			return 314; // 红色魔女之森
		}
		if (role_level <= 90) {
			return 314; // 红色魔女之森
		}
		return 0;
	}

	if (role_level <= 100) {
		if (role_level <= 95) {
			return 291100293; // 全蚀市场
		}
		if (role_level <= 98) {
			return 291100293; // 搏击俱乐部
		}
	}
	if (role_level <= 109) {
		if (role_level <= 100) {
			return 100002975; // 圣域外围
		}
		if (role_level <= 102) {
			return 100002976; // 圣域中心
		}
		if (role_level <= 103) {
			return 100002977; // 泽尔峡谷
		}
		if (role_level <= 104) {
			return 100002978; // 洛仑山
		}
		if (role_level <= 105) {
			return 100002979; // 白色雪原
		}
		if (role_level <= 106) {
			return 100002980; // 贝奇的空间
		}
		if (role_level <= 107) {
			return 100002981;
		}
		if (role_level <= 108) {
			return 100002982;
		}
		if (role_level <= 109) {
			return 100002983;
		}

		return 0;
	}
	return 0;
}
