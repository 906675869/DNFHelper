#pragma once

namespace game 
{
	enum AUTO_MODEL {COINS=0,STORY=1,WY=2};

	struct Config {
		int autoModel; // �Զ�ģʽ
		bool floatHarm; // ���㱶��
		bool hookHarm; // �̶��˺�
		bool pureInvincible;// ͸���޵�
		bool btInvincible;// �����޵�
		bool speed; // ����΢��
		bool firePerson; // ������ͼ
		bool autoPick; // �Զ�����
		bool fullScreenHook;// ȫ������
		bool floatCooldown;// ������ȴ
		bool skillNoCd; // ������cd
	};

	extern Config config;

	struct ProcessConfig {
		// �Ƿ��Ѿ���ʼ����Į
		bool dmInit;

		// ��Ϸpid
		DWORD gamePid;
		// ��ǰ����pid
		DWORD currentPid;


	};

	extern ProcessConfig processConfig;



	struct CoordinateStruct {
		int x;
		int y;
		int z;
	};
}

