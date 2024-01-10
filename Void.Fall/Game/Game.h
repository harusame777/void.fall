#pragma once
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
class P_main_Player;
class E_001_enemy;
class E_002_enemy;
class E_003_enemy;
class E_004_enemy;
class Map_home1;
class Ca_maincamera;
class IEnemy;
class Imap;
class M_parts1;
class M_parts2;
class M_parts3;
class M_parts4;
class M_parts4_sub;
class M_parts5;
class mapMaker;
class Ob_savepoint;
///////////////////////////////////////////////////////////
class Game : public IGameObject
{
public:
	enum PlayerPlaystate{
		en_play,
		en_down
	};
	//�֐��錾
	Game() {}
	~Game();
	void DeleteMap();
	void DeleteEnemy();
	void DeleteItem();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void Font();
	void Save();
	void Down();
	void Delete_EnemyVec(const int num);
	bool RelocationEnemy();
	void DownSet(){
		m_playstate = en_down;
	}
	//�����o�֐��錾
	std::vector<IEnemy*> m_EnemyList;
	std::vector<M_parts4*> m_Map4List;
	std::vector<E_004_enemy*> m_MapLock;
	std::vector<Ob_savepoint*> m_saveList;
	Map_home1* m_home1 = nullptr;//home
	P_main_Player* m_player = nullptr;//�v���C���[
	E_001_enemy* m_enemy_001 = nullptr; //�G�l�~�[001
	E_002_enemy* m_enemy_002 = nullptr; //�G�l�~�[002
	E_003_enemy* m_enemy_003 = nullptr; //�G�l�~�[003
	E_004_enemy* m_enemy_004 = nullptr; //�G�l�~�[004
	M_parts1* m_mapparts1 = nullptr;
	M_parts2* m_mapparts2 = nullptr;
	M_parts3* m_mapparts3 = nullptr;
	M_parts4* m_mapparts4 = nullptr;
	M_parts5* m_mapparts5 = nullptr;
	mapMaker* m_mapMaker = nullptr;
	Ca_maincamera* m_camera = nullptr;//�J����
	IEnemy* m_ienemy = nullptr;
	LevelRender m_levelrender;//���x�������_�[
	FontRender m_fontrender;//�X�v���C�g�����_�[
	Ob_savepoint* m_savepoint = nullptr;
	Vector3 Recoverypos = { 0.0,-200.0,0.0 };
	PlayerPlaystate m_playstate = en_play;
	int m_Nowsavepointnum = -1;//���݂̃Z�[�u�i���o�[
	int m_savenum = 0;//�Z�[�u�|�C���g��
	int m_numenemy = 0; //�G�l�~�[��
	int map4_num = 0;
	int SummonEnemynum = 0;
	int GoalLockNum = 0;
};

