#pragma once
class P_main_Player;
class B_homingbullet : public IGameObject
{
public:
///////////////////////////////////////////////////////////
	B_homingbullet() {};
	~B_homingbullet() {};
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void Movebullet();
	void Inpact();
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
//メンバ関数宣言
	float bullettime = 10.0f;
	CollisionObject* m_collisionObject;						//コリジョンオブジェクト。
	P_main_Player* m_player = nullptr;                      //プレイヤー
	AnimationClip m_animationclips[enAnimationClip_Num];    //アニメーションクリップ
	ModelRender* m_modelrender = nullptr;					//モデルレンダー
	Vector3 m_position;										//座標
	Vector3 m_velocity;										//速度
	Vector3 m_targetPosition;								//追尾対象座標


};

