#pragma once
class P_main_Player;
class B_homingbullet : public IGameObject
{
public:
///////////////////////////////////////////////////////////
	enum EnShooter {
		enShooter_None,
		enShooter_Player,			//プレイヤー。
		enShooter_Enemy			//エネミー。
	};
	B_homingbullet() {};
	~B_homingbullet() {};
	bool Start();
	void Update();
	void Rotation();
	void Render(RenderContext& rc);
	void Movebullet();
	void Inpacttime();
	void Inpacthit();
	void deletebullet();				//弾丸消去
	void CalcVelocity(const float speed, const float curvatureRadius,
		const float damping);
///////////////////////////////////////////////////////////
//アニメーション類/////////////////////////////////////////
	//アニメーションステート
	enum EnAnimationClip {
		enAnimationClip_Move,//bullet移動
		enAnimationClip_Num//アニメーション数
	};
//アニメーション関数///////////////////////////////////////
	void PlayAnimation();
///////////////////////////////////////////////////////////
//初期設定系統
	void SetVelocity(const Vector3& velocity)
	{
		m_velocity = velocity;
	}
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	void SetTarget(const Vector3& targetPosition)
	{
		m_targetPosition = targetPosition;
	}
	void SetEnShooter(const EnShooter enShooter)
	{
		m_shooter = enShooter;
	}
//メンバ関数宣言
	float bullettime = 6.0f;
	float m_deleteTimer = 0.0f;								//消去ディレイ
	bool m_isDelete = false;								//消去ディレイに使うやつ
	const float deletetimer = 0.1f;							//ディレイタイマー
	Quaternion m_rotation;									//回転
	CollisionObject* m_collisionObject;						//コリジョンオブジェクト。
	Vector3	m_forward = Vector3::AxisZ;						//エネミーの正面ベクトル。
	Vector3	m_scale = Vector3::One;							//大きさ。
	Vector3 targetPosition;
	P_main_Player* m_player = nullptr;                      //プレイヤー
	AnimationClip m_animationclips[enAnimationClip_Num];    //アニメーションクリップ
	ModelRender m_modelrender;								//モデルレンダー
	Vector3 m_position;										//座標
	Vector3 m_velocity;										//速度
	Vector3 m_targetPosition;								//追尾対象座標
	EnShooter m_shooter = enShooter_None;                                    //射撃者設定
};

