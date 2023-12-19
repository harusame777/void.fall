#pragma once
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
class P_main_Player;
class E_001_enemy;
class E_002_enemy;
class E_003_enemy;
class Map_home1;
class Ca_maincamera;
class IEnemy;
class Imap;
class M_parts1;
class M_parts2;
class M_parts3;
class mapMaker;
///////////////////////////////////////////////////////////
class Game : public IGameObject
{
public:
	//関数宣言
	Game() {}
	~Game() {}
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void Delete_EnemyVec(const int num);
	//メンバ関数宣言
	std::vector<IEnemy*> m_EnemyList;
	Map_home1* m_home1 = nullptr;//home
	P_main_Player* m_player = nullptr;//プレイヤー
	E_001_enemy* m_enemy_001 = nullptr; //エネミー001
	E_002_enemy* m_enemy_002 = nullptr; //エネミー002
	E_003_enemy* m_enemy_003 = nullptr; //エネミー003
	M_parts1* m_mapparts1 = nullptr;
	M_parts2* m_mapparts2 = nullptr;
	M_parts3* m_mapparts3 = nullptr;
	mapMaker* m_mapMaker = nullptr;
	Ca_maincamera* m_camera = nullptr;//カメラ
	IEnemy* m_ienemy = nullptr;
	LevelRender m_levelrender;//レベルレンダー
	FontRender m_fontrender;//スプライトレンダー
	int m_numenemy = 0; //エネミー数
};

