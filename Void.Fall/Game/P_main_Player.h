#pragma once
class P_main_Player : public IGameObject
{
public:
	//ステート
	enum EnPlayerState {
		enPlayerState_Idle,//待機。
		enPlayerState_Walk,//歩き。
		enPlayerState_Idlefry,//ホバー待機。
		enPlayerState_Walkfry,//ホバー歩き。
	};
	//関数宣言
	P_main_Player() {}
	~P_main_Player() {}
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void Move();//移動
	void Movefry();//ホバー
	void Rotation();//回転
	void ManageState();//ステート遷移処理
	//アニメーション類/////////////////////////////////////////
//アニメーションステート
	enum EnAnimationClip {
		enAnimationClip_Walk,//歩き
		//enAnimationClip_Attack,//攻撃
		enAnimationClip_Idle,//棒立ち
		enAnimationClip_Num//アニメーション数
	};
	//アニメーション関数///////////////////////////////////////
	void PlayAnimation();
	//ステート遷移関数/////////////////////////////////////////
	void ProcessCommonStateTransition();//共通処理
	void ProcessIdleStateTransition();//待機遷移
	void ProcessWalkStateTransition();//歩き遷移
	bool IsEnableMove() const//通常移動状態かの確認
	{
		return m_playerstate != enPlayerState_Idlefry &&
			m_playerstate != enPlayerState_Walkfry;
	}
	bool IsEnableMovefry() const//ホバー状態かの確認
	{
		return m_playerstate != enPlayerState_Idle &&
			m_playerstate != enPlayerState_Walk;
	}
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
	void Setrotarion(const Quaternion& rotation)//回転値
	{
		m_rotation = rotation;
	}
	void Setscale(const Vector3& scale)//座標
	{
		m_scale = scale;
	}
	//メンバ関数宣言
	AnimationClip m_animationclips[enAnimationClip_Num];     //アニメーションクリップ
	Vector3 m_position;										//座標
	Vector3 m_scale = Vector3::One;							//大きさ
	Quaternion m_rotation;									//回転
	Vector3 m_movespeed;									//移動速度
	Vector3	m_forward = Vector3::AxisZ;						//プレイヤーの正面ベクトル。
	EnPlayerState m_playerstate = enPlayerState_Idle;		//プレイヤーステート
	CharacterController	m_charaCon;							//キャラクターコントローラー。
	ModelRender* m_modelrender = nullptr;					//モデルレンダー
	//変数宣言
	int m_hp = 3;											//HP
	bool m_fryflag = false;									//ホバーフラグ1.true飛べる||0.false飛べない
	int m_fry = 100;										//ホバーゲージ
	int m_frymax = 100;										//ホバーゲージ最大
	float timerfry = 0.0f;
	void Addfry()
	{
		if (IsEnableMovefry() == false)
		{
			if (timerfry > 0.0f)
			{
				return;
			}
			if (m_fry > 100)
			{
				return;
			}
			m_fry++;
			timerfry = 0.1f;
		}
		else if (IsEnableMove() == false)
		{
			if (timerfry > 0.0f)
			{
				return;
			}
			m_fry--;
			timerfry = 0.1f;
		}
	}
};

