#pragma once

#include "camera/SpringCamera.h"

class P_main_Player;

class Ca_maincamera : public IGameObject
{
public:
	//�֐��錾
	Ca_maincamera() {}
	~Ca_maincamera(){}
	bool Start();
	void Update();
	void CamposUpdate();
	void ProcessLockOn();
	void DeleteCamera()
	{
		DeleteGO(this);
	}
	//�����o�֐��錾
	P_main_Player* m_player = nullptr;
	Vector3 m_Campos = Vector3::One;
	SpringCamera m_springCamera;	//�΂˃J�����B
};

