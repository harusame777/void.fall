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
	//関数宣言
	Game() {}
	~Game() {}
	bool Start();
	//メンバ関数宣言
	Map_home1* m_home1 = nullptr;//home
	P_main_Player* m_player = nullptr;//プレイヤー
	E_001_enemy* m_enemy_001 = nullptr; //エネミー001
	E_002_enemy* m_enemy_002 = nullptr; //エネミー002
	Ca_maincamera* m_camera = nullptr;//カメラ
	LevelRender m_levelrender;//レベルレンダー
	FontRender* m_fontrender = nullptr;//スプライトレンダー
	int m_numenemy = 0; //エネミー数
};

