#pragma once
///////////////////////////////////////////////////////////
class P_main_Player;
///////////////////////////////////////////////////////////
class E_001_enemy : public IGameObject
{
public:
	//ステート
	enum EnEnemyState {
		enEnemyState_Idle,//待機
		enEnemyState_Chase,//追跡
	};
	//ステート遷移関数/////////////////////////////////////////
	void ManageState();//遷移処理
	void ProcessCommonStateTransition();//共通遷移処理
	void ProcessIdleStateTransition();//待機遷移
	void ProcessChaseStateTransition();//追跡遷移
	///////////////////////////////////////////////////////////
	E_001_enemy(){}
	~E_001_enemy(){}
	bool Start();
	void Update();                                         //アップデート
	void Render(RenderContext& rc);                        //モデルレンダー
	void Chase();                                          //追跡
	void Rotation();                                       //回転
	const bool SearchPlayer() const;                       //プレイヤー探知
///////////////////////////////////////////////////////////
//初期設定系統
	void Setposition(const Vector3& position)//座標
	{
		m_position = position;
	}
	const Vector3& Getposition() const//座標取得プログラム
	{
		return m_position;
	}
	void Setrotation(const Quaternion& rotation)//回転値
	{
		m_rotation = rotation;
	}
	void Setscale(const Vector3& scale)//座標
	{
		m_scale = scale;
	}
	void SetHP(const int hp)//初期HP
	{
		m_hp = hp;
	}
	//メンバ関数宣言
	ModelRender* m_modelrender = nullptr;					//モデルレンダー
	Vector3 m_position;										//座標
	Vector3 m_scale = Vector3::One;							//大きさ
	Quaternion m_rotation;									//回転
	Vector3 m_movespeed;									//移動速度
	Vector3	m_forward = Vector3::AxisZ;						//エネミーの正面ベクトル。
	EnEnemyState m_enemystate = enEnemyState_Idle;          //エネミーステート
	CharacterController	m_charaCon;							//キャラクターコントローラー。
	P_main_Player* m_player = nullptr;                      //プレイヤー
	int m_hp = 0;                                           //HP
	float m_chaseTimer = 0.0f;						        //追跡タイマー。
	float m_idleTimer = 0.0f;							    //待機タイマー。
};

