#pragma once
///////////////////////////////////////////////////////////
#include "Level3DRender/LevelRender.h"
///////////////////////////////////////////////////////////
class P_main_Player;
class E_001_enemy;
class E_002_enemy;
class Map_home1;
class Ca_maincamera;
///////////////////////////////////////////////////////////
class Game : public IGameObject
{
public:
	//�֐��錾
	Game() {}
	~Game() {}
	bool Start();
	//�����o�֐��錾
	Map_home1* m_home1 = nullptr;//home
	P_main_Player* m_player = nullptr;//�v���C���[
	E_001_enemy* m_enemy_001 = nullptr; //�G�l�~�[001
	E_002_enemy* m_enemy_002 = nullptr; //�G�l�~�[002
	Ca_maincamera* m_camera = nullptr;//�J����
	LevelRender m_levelrender;//���x�������_�[
	FontRender* m_fontrender = nullptr;//�X�v���C�g�����_�[
	int m_numenemy = 0; //�G�l�~�[��
};

