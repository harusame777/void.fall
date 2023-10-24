#pragma once
class IEnemy;
class P_main_Player;
class B_normalbullet : public IGameObject
{
///////////////////////////////////////////////////////////
public:
	B_normalbullet() {};
	~B_normalbullet() {};
	bool Start();
	void Update();						//アップデート
	void Rotation();					//回転
	void Movebullet();					//弾丸移動
	void Render(RenderContext& rc);		//モデルレンダー
	void Inpacttime();					//弾丸自然消去
	void Inpacthit();					//弾丸衝突
	void deletebullet();				//弾丸消去
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
	///////////////////////////////////////////////////////////
//初期設定系統
	void SetVelocity(const Vector3& velocity)//速度初期設定
	{
		m_velocity = velocity;
	}
	void SetPosition(const Vector3& position)//位置初期設定
	{
		m_position = position;
	}
	void Setrotation(const Quaternion& rotation)//回転値初期設定
	{
		m_rotation = rotation;
	}
//メンバ関数宣言
	float bullettime = 1.5f;								//自然消去タイマー
	float m_deleteTimer = 0.0f;								//消去ディレイ
	bool m_isDelete = false;								//消去ディレイに使うやつ
	const float deletetimer = 0.1f;							//ディレイタイマー
	AnimationClip m_animationclips[enAnimationClip_Num];    //アニメーションクリップ
	Vector3	m_forward = Vector3::AxisZ;						//エネミーの正面ベクトル。
	ModelRender* m_modelrender = nullptr;					//モデルレンダー
	CollisionObject* m_collisionObject;						//コリジョンオブジェクト。
	P_main_Player* m_player = nullptr;                      //プレイヤー
	Quaternion	m_rotation;									//回転。
	Vector3 m_position;										//座標
	Vector3 m_velocity;										//速度
	Vector3	m_scale = Vector3::One;							//大きさ。
};

