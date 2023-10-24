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
	void Update();
	void Rotation();
	void Movebullet();
	void Render(RenderContext& rc);
	void Inpacttime();
	void Inpacthit();
	void Delay(); //弾丸消去ディレイ
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
	void SetVelocity(const Vector3& velocity)
	{
		m_velocity = velocity;
	}
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	void Setrotation(const Quaternion& rotation)//回転値
	{
		m_rotation = rotation;
	}
//メンバ関数宣言
	float bullettime = 1.5f;
	float bullettdelaytime = 2.0f;
	AnimationClip m_animationclips[enAnimationClip_Num];    //アニメーションクリップ
	Vector3	m_forward = Vector3::AxisZ;						//エネミーの正面ベクトル。
	ModelRender* m_modelrender = nullptr;					//モデルレンダー
	CollisionObject* m_collisionObject;						//コリジョンオブジェクト。
	P_main_Player* m_player = nullptr;                      //プレイヤー
	IEnemy* m_ienemy = nullptr;
	Quaternion	m_rotation;									//回転。
	Vector3 m_position;										//座標
	Vector3 m_velocity;										//速度
	Vector3	m_scale = Vector3::One;							//大きさ。
};

