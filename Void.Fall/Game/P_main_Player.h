#pragma once
class IEnemy;
class Game;
class B_normalbullet;
class P_main_Player : public IGameObject
{
public:
	//ステート
	enum EnPlayerState {
		enPlayerState_Idle,				//待機。
		enPlayerState_Walk,				//歩き。
		enPlayerState_Attack,			//攻撃
		enPlayerState_Avoidance,		//回避
		enPlayerState_Down,				//ダウン
		enPlayerState_ReceiveDamage,    //被ダメージ。
	};
	//関数宣言
	P_main_Player() {}
	~P_main_Player() {}
	bool Start();
	void Update();						//アップデート
	void Render(RenderContext& rc);		//モデルレンダー
	void Move();						//移動
	void Rotation();					//回転
	void ManageState();					//ステート遷移処理
	void Collision();					//本体の当たり判定
	void Lockon();						//ロックオン
	void Takeaim();						//ロックオンに対する位置取得
	void Avoidance();					//回避
	void AvoidanceTex();				//回避スキン変更
	void NormalTex();					//通常スキン変更
	//アニメーション類/////////////////////////////////////////
//アニメーションステート
	enum EnAnimationClip {
		enAnimationClip_Walk,			//歩き
		enAnimationClip_Attack,			//攻撃
		enAnimationClip_Idle,			//棒立ち
		enAnimationClip_ReceiveDamage,  //被ダメージ。
		enAnimationClip_Down,            //ダウン
		enAnimationClip_Num				//アニメーション数
	};
	enum EnAnimationClip_sub {
		enAnimationClip_Avoidance,		//回避
		enAnimationClip_sub_Num				//アニメーション数
	};
	//アニメーション関数///////////////////////////////////////
	void PlayAnimation();
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	//ステート遷移関数/////////////////////////////////////////
	void ProcessCommonStateTransition();	//共通処理
	void ProcessIdleStateTransition();		//待機遷移
	void ProcessWalkStateTransition();		//歩き遷移
	void ProcessAttackStateTransition();	//攻撃遷移
	void ProcessAvoidanceStateTransition(); //回避遷移
	void ProcessReceiveDamageStateTransition();//被ダメ遷移
	void ProcessDownStateTransition();//ダウン遷移
	bool IsEnableMove() const				//移動できるかどうか
	{
		return m_playerstate != enPlayerState_Attack &&
			m_playerstate != enPlayerState_Avoidance &&
			m_playerstate != enPlayerState_Idle &&
			m_playerstate != enPlayerState_Walk;
	}
	///////////////////////////////////////////////////////////
	//初期設定系統
	void Setposition(const Vector3& position)				//座標
	{
		m_position = position;
	}
	const Vector3& Getposition() const						//座標取得プログラム
	{
		return m_position;
	}
	void Setrotarion(const Quaternion& rotation)			//回転値
	{
		m_rotation = rotation;
	}
	void Setscale(const Vector3& scale)						//座標
	{
		m_scale = scale;
	}
	const bool IsLockOn() const
	{
		return m_isLockOn;
	}
	const Vector3& GetTargetPosition() const
	{
		return m_targetPosition;
	}
	//メンバ関数宣言
	std::vector<Vector3*> m_enemyPositionList;
	IEnemy* m_ienemy = nullptr;
	AnimationClip m_animationclips[enAnimationClip_Num];	//アニメーションクリップ
	AnimationClip m_animationclips_sub[enAnimationClip_sub_Num];//アニメーションクリップ
	Vector3 m_position;										//座標
	Vector3 m_scale = Vector3::One;							//大きさ
	Quaternion m_rotation;									//回転
	Vector3 m_movespeed;									//移動速度
	Vector3	m_forward = Vector3::AxisZ;						//プレイヤーの正面ベクトル。
	EnPlayerState m_playerstate = enPlayerState_Idle;		//プレイヤーステート
	CharacterController	m_charaCon;							//キャラクターコントローラー。
	ModelRender* m_modelrender = nullptr;					//モデルレンダー
	SpriteRender m_spriterender;
	Vector3 m_targetPosition = Vector3::Zero;
	Game* m_game = nullptr;
	//変数宣言
	bool m_isTakeAim = false;
	bool m_isLockOn = false;
	int m_numenemy = 0;
	int m_hp = 3;											//HP
	float m_Avoidancetimer = 0.0f;							//回避タイマー
	float Avoidancetime = 0.2f;								//回避時間
	float m_Avoidbreaktimer = 0.0f;							//回避クールタイマー
	float Avoidbreaktime = 2.0f;							//回避クールタイム時間
	float m_mutekitimer = 0.0f;								//無敵タイマー
	float mutekitime = 2.0f;								//無敵時間
};

