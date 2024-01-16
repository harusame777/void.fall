#pragma once
#include "Imap.h"
class P_main_Player;
class Game;
class GameClear;
class M_parts5 : public IGameObject
{
public:
	bool Start();
	void Update();
	void Collision();										//本体の当たり判定
	void Lock();
	void Render(RenderContext& rc);
	//初期設定系統
	void Setposition(const Vector3& position)//座標
	{
		m_position = position;
	}
	void Setrotarion(const Quaternion& rotation)//回転値
	{
		m_rotation = rotation;
	}
	void Setscale(const Vector3& scale)//座標
	{
		m_scale = scale;
	}
	void SetPhysics() {
		//当たり判定作成
		m_physicsStaticObject.CreateFromModel(m_modelrender.GetModel(), m_modelrender.GetModel().GetWorldMatrix());
	}
	CollisionObject* m_collisionObject;		//コリジョンオブジェクト。
	Game* m_game = nullptr;
	P_main_Player* m_player = nullptr;
	GameClear* m_gameclear = nullptr;
	Vector3 m_position;//座標
	Vector3 m_scale = Vector3::One;//大きさ
	Quaternion m_rotation;//回転
	ModelRender m_modelrender;//モデルレンダー
	PhysicsStaticObject m_physicsStaticObject;//当たり判定
	bool Deletemap = true;
	int GoalLockNum = 0;
};

