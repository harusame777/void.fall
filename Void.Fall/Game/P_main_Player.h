#pragma once
class IEnemy;
class B_normalbullet;
class P_main_Player : public IGameObject
{
public:
	//ステート
	enum EnPlayerState {
		enPlayerState_Idle,				//待機。
		enPlayerState_Walk,				//歩き。
		enPlayerState_Attack			//攻撃
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
	void Lockon();						//ロックオン
	void Takeaim();						//ロックオンに対する位置取得
	//アニメーション類/////////////////////////////////////////
//アニメーションステート
	enum EnAnimationClip {
		enAnimationClip_Walk,			//歩き
		enAnimationClip_Attack,			//攻撃
		enAnimationClip_Idle,			//棒立ち
		enAnimationClip_Num				//アニメーション数
	};
	//アニメーション関数///////////////////////////////////////
	void PlayAnimation();
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	//ステート遷移関数/////////////////////////////////////////
	void ProcessCommonStateTransition();	//共通処理
	void ProcessIdleStateTransition();		//待機遷移
	void ProcessWalkStateTransition();		//歩き遷移
	void ProcessAttackStateTransition();	//攻撃遷移
	bool IsEnableMove() const				//移動できるかどうか
	{
		return true;
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
	//メンバ関数宣言
	AnimationClip m_animationclips[enAnimationClip_Num];	//アニメーションクリップ
	Vector3 m_position;										//座標
	Vector3 m_scale = Vector3::One;							//大きさ
	Quaternion m_rotation;									//回転
	Vector3 m_movespeed;									//移動速度
	Vector3	m_forward = Vector3::AxisZ;						//プレイヤーの正面ベクトル。
	EnPlayerState m_playerstate = enPlayerState_Idle;		//プレイヤーステート
	CharacterController	m_charaCon;							//キャラクターコントローラー。
	ModelRender* m_modelrender = nullptr;					//モデルレンダー
	IEnemy* m_ienemy;
	//変数宣言
	int m_hp = 3;											//HP
};

