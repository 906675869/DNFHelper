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
			cout << "��������-> ��������" << task.taskName.c_str() <<endl;
			cout << "��������-> ��������" << task.taskConditional.c_str() <<endl;
			cout << "��������-> ����Id" << task.taskId <<endl;
		}
		// ����������
		if (task.taskId == 0) {
			if (!refreshTask) {
				cout << "��ʱ��������߿�����" << endl;
				pk.RoleList();
				Sleep(200);
				pk.ChooseRole(as.chooseRoleNum);
				Sleep(500);
				refreshTask = TRUE;
				continue;
			}
			else {
				cout << "�޷��������ִ����Ӧ�ȼ���ͼ" << endl;
				return HighestMap();
			}
		}
		refreshTask = FALSE;
		if (task.taskId == 3509 || task.taskId == 5943) {
			cout << "�޷��������ִ����Ӧ�ȼ���ͼ" << endl;
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
			cout << "���������޷���ɣ�ִ����ߵȼ���ͼ" << endl;
			return mapId;
		}
		return mapId;

	}
}

TaskStruct Task::MainLineTask()
{
	ULONG64 task_addr = rw.ReadLong(�����ַ);
	ULONG64	start = rw.ReadLong(task_addr + ȫ�������׵�ַ);
	ULONG64 end = rw.ReadLong(task_addr + ȫ������β��ַ);
	int	num = int((end - start) / 8);
	for (int i = 0; i < num; i++) {
		ULONG64 task_ptr = rw.ReadLong(start + i * 8);
		int	task_type = rw.ReadInt(task_ptr + ��������);
		if (task_type == 0) {
			int task_length = rw.ReadInt(task_ptr + �����С);
			wstring task_name;
			if (task_length > 7) {
				ULONG64 tmp = rw.ReadLong(task_ptr + 16);
				task_name = UnicodeToAnsi(rw.ReadBytes(tmp, 100));
			}
			else {
				task_name = UnicodeToAnsi(rw.ReadBytes(task_ptr + 16, 100));
			}
			wstring task_conditional = UnicodeToAnsi(rw.ReadBytes(rw.ReadLong(task_ptr + ��������), 100));
				// # ������
			int	task_id = rw.ReadInt(task_ptr);
			return { task_name , task_conditional , task_id };
		}
		
	}

	return TaskStruct();
}

TaskSkip Task::CouldSkip(int taskId)
{
	ULONG64 task_addr = rw.ReadLong(�����ַ);
	ULONG64 start = rw.ReadLong(task_addr + �ѽ������׵�ַ);
	ULONG64 end = rw.ReadLong(task_addr + �ѽ�����β��ַ);
	int	num = int((end - start) / 16);

	for (int i = 0; i < num; i++) {
		ULONG64 task_ptr = rw.ReadLong(start + i * 16);
		if (rw.ReadInt(task_ptr) == taskId){
			int task_level = rw.ReadInt(task_ptr + ����ȼ�);
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
	ULONG64 task_addr = rw.ReadLong(�����ַ);
	ULONG64 start = rw.ReadLong(task_addr + �ѽ������׵�ַ);
	ULONG64 end = rw.ReadLong(task_addr + �ѽ�����β��ַ);
	int	num = int((end - start) / 16);

	for (int i = 0; i < num; i++) {
		ULONG64 task_ptr = rw.ReadLong(start + i * 16);
		if (rw.ReadInt(task_ptr) == taskId) {
			ULONG64 task_data = rw.ReadLong(task_ptr + ���񸱱�);
			return rw.ReadInt(task_data);
		}
	}
	return 0;
}

void Task::SubmitTask()
{
	ULONG64 task_addr = rw.ReadLong(�����ַ);
	ULONG64	start = rw.ReadLong(task_addr + ȫ�������׵�ַ);
	ULONG64 end = rw.ReadLong(task_addr + ȫ������β��ַ);
	int	num = int((end - start) / 8);

	for (int i = 0; i < num; i++) {
		ULONG64 task_ptr = rw.ReadLong(start + i * 8);
		int	task_type = rw.ReadInt(task_ptr + ��������);
		if (task_type == 0) {
			int task_id = rw.ReadInt(task_ptr);
			cl.SubmitMissionCall(task_id);
		}
	}

	start = rw.ReadLong(task_addr + �ѽ������׵�ַ);
	end = rw.ReadLong(task_addr + �ѽ�����β��ַ);
	num = int((end - start) / 16);
	for (int i = 0; i < num; i++) {
		int task_ptr = rw.ReadLong(start + i * 16);
		int	task_type = rw.ReadInt(task_ptr + ��������);
		if(task_type == 0){
			int task_id = rw.ReadInt(task_ptr);
			cl.SubmitMissionCall(task_id);
		}
	}
}

int Task::FinishStatus(int taskId)
{
	ULONG64 task_addr = rw.ReadLong(�����ַ);
	ULONG64	start = rw.ReadLong(task_addr + �ѽ������׵�ַ);
	ULONG64 end = rw.ReadLong(task_addr + �ѽ�����β��ַ);
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
				tmp_arr[1] = int(the_rest / 512);// ���ã�
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
			return 3; // ��������
		}
		if (role_level <= 5) {
			return 5; // ��������
		}
		if (role_level <= 8) {
			return 6; // �Ͷ���������
		}
		if (role_level <= 11) {
			return 9; // ��˪�İ�����
		}
		if (role_level <= 13) {
			return 7; // ������
		}
		if (role_level <= 15) {
			return 8; // ���������
		}if (role_level <= 17) {
			return 1000; // ������������
		}return 0;
	}

	if (role_level <= 23) {
		if (role_level <= 18) {
			return 1000; // 10 ����֮��
		}if (role_level <= 19) {
			return 12; // ��ż����
		}if (role_level <= 20) {
			return 13; // ʯ������
		}if (role_level <= 21) {
			return 14; // �ڰ�����
		}if (role_level <= 22) {
			return 17; // ���ճ�
		}if (role_level <= 23) {
			return 15; // ��������
		}return 0

			; // ��׵
	}

	if (role_level <= 29) {
		if (role_level <= 24) {
			return 15; // 21 �����Χ
		}
		if (role_level <= 25) {
			return 22; // ��������
		}
		if (role_level <= 26) {
			return 23; // ����
		}
		if (role_level <= 27) {
			return 24; // ����
		}
		if (role_level <= 28) {
			return 25; // ��һ��׵
		}
		if (role_level <= 29) {
			return 26; // �ڶ���׵
		}
		return 0;

	}
	//; // ���������
	if (role_level <= 35) {
		if (role_level <= 30) {
			return 26; // 31  ǳ��֮��
		}
		if (role_level <= 31) {
			return 32; // ֩�붴Ѩ
		}
		if (role_level <= 32) {
			return 150; // ֩������
		}
		if (role_level <= 33) {
			return 151; // Ӣ��ڣ
		}
		if (role_level <= 34) {
			return 35; // ������Ĺ��
		}
		if (role_level <= 35) {
			return 34; // ����Ѩ
		}
		return 0;
		//; // ��̳
	}
	if (role_level <= 39) {
		if (role_level <= 36) {
			return 34; // 152 �����ļ�̳
		}
		if (role_level <= 37) {
			return 153; // �ƽ��
		}
		if (role_level <= 38) {
			return 154; // Զ��ĹѨ�
		}
		if (role_level <= 39) {
			return 154; // Զ��ĹѨ�
		}
		return 0;
		// ѩɽ
	}

	if (role_level <= 45) {
		if (role_level <= 40) {
			return 154; // 40 ɽ��
		}if (role_level <= 41) {
			return 41; // ��������
		}if (role_level <= 42) {
			return 42; // �����쾮
		}if (role_level <= 44) {
			return 141; // ����ӵ�������
		}if (role_level <= 45) {
			return 141; // ����ӵ�������
		}
		return 0;

		//; // �̶�
	}

	if (role_level <= 49) {
		if (role_level <= 46) {
			return 141; // 61  �̶���������
		}
		if (role_level <= 47) {
			return 50; // ����ĵ���
		}
		if (role_level <= 48) {
			return 51; // ����֮�������
		}
		if (role_level <= 49) {
			return 53; // �ɻ�֮��
		}
		return 0;
	}

	if (role_level <= 53) {
		if (role_level <= 50) {
			return 53; // 144  �㾧ɭ��
		}
		if (role_level <= 51) {
			return 145; // ����ɭ��
		}
		if (role_level <= 52) {
			return 146; // ˮ������
		}
		if (role_level <= 53) {
			return 148; // ��ڤ����
		}
		return 0;
	}

	if (role_level <= 58) {
		if (role_level <= 54) {
			return 148; // 156 Ģ��ׯ԰
		}
		if (role_level <= 55) {
			return 157; // �Ϻ�ĳ�Ѩ
		}
		if (role_level <= 56) {
			return 158; // ����֮��
		}
		if (role_level <= 57) {
			return 159; // �ն�����ɽ���
		}
		if (role_level <= 58) {
			return 160; // ������Ϣ��
		}
		return 0;
	}


	if (role_level <= 62) {
		if (role_level <= 59) {
			return 160; // 162  '�������
		}
		if (role_level <= 60) {
			return 163; // GBLŮ���
		}
		if (role_level <= 61) {
			return 164; // ������ֳ��
		}
		if (role_level <= 62) {
			return 164; // ������ֳ��
		}
		return 0;
	}

	if (role_level <= 70) {
		if (role_level <= 63) {
			return 164; // 80  '������Χ
		}
		if (role_level <= 64) {
			return 81; // ���ض���
		}
		if (role_level <= 65) {
			return 82; // ��������
		}
		if (role_level <= 66) {
			return 88; // ���ر���
		}
		if (role_level <= 67) {
			return 88; // ���ر���
		}
		if (role_level <= 68) {
			return 83; // ҹ��Ϯ��ս
		}
		if (role_level <= 69) {
			return 84; // �������վ
		}
		if (role_level <= 70) {
			return 85; // ׷������ս
		}
		return 0;
		//; // �����г�
	}

	if (role_level <= 74) {
		if (role_level <= 71) {
			return 85; // 86 �г��ϵĺ���
		}
		if (role_level <= 71) {
			return 87; // ���������
		}
		if (role_level <= 73) {
			return 92; // ������˹
		}
		if (role_level <= 74) {
			return 93; // ���Ǹߵ�
		}
		return 0;
	}

	if (role_level <= 80) {
		if (role_level <= 75) {
			return 93; // 70 ����֮��
		}
		if (role_level <= 76) {
			return 71; // ����֮Դ
		}
		if (role_level <= 77) {
			return 72; // ������֮��
		}
		if (role_level <= 78) {
			return 74; // ��������
		}
		if (role_level <= 79) {
			return 75; // ���ձ���
		}
		if (role_level <= 80) {
			return 76; // �ݶ�
		}
		return 0;
	}
	if (role_level <= 85) {
		if (role_level <= 81) {
			return 76; // 102 ��³�ȷ���վ
		}
		if (role_level <= 82) {
			return 103; // ����˹����վ
		}
		if (role_level <= 85) {
			return 104; // �����Ϸ���վ
		}
		return 0;
	}
	if (role_level <= 86) {
		return 192; // ����֮��
	}

	if (role_level <= 90) {
		if (role_level <= 87) {
			return 310; // ʱ��㳡
		}
		if (role_level <= 88) {
			return 312; // �ֲ�����Ϣ��
		}
		if (role_level <= 89) {
			return 314; // ��ɫħŮ֮ɭ
		}
		if (role_level <= 90) {
			return 314; // ��ɫħŮ֮ɭ
		}
		return 0;
	}

	if (role_level <= 100) {
		if (role_level <= 95) {
			return 291100293; // ȫʴ�г�
		}
		if (role_level <= 98) {
			return 291100293; // �������ֲ�
		}
	}
	if (role_level <= 109) {
		if (role_level <= 100) {
			return 100002975; // ʥ����Χ
		}
		if (role_level <= 102) {
			return 100002976; // ʥ������
		}
		if (role_level <= 103) {
			return 100002977; // ���Ͽ��
		}
		if (role_level <= 104) {
			return 100002978; // ����ɽ
		}
		if (role_level <= 105) {
			return 100002979; // ��ɫѩԭ
		}
		if (role_level <= 106) {
			return 100002980; // ����Ŀռ�
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
