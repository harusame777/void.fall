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
	void Collision();										//�{�̂̓����蔻��
	void Lock();
	void Render(RenderContext& rc);
	//�����ݒ�n��
	void Setposition(const Vector3& position)//���W
	{
		m_position = position;
	}
	void Setrotarion(const Quaternion& rotation)//��]�l
	{
		m_rotation = rotation;
	}
	void Setscale(const Vector3& scale)//���W
	{
		m_scale = scale;
	}
	void SetPhysics() {
		//�����蔻��쐬
		m_physicsStaticObject.CreateFromModel(m_modelrender.GetModel(), m_modelrender.GetModel().GetWorldMatrix());
	}
	CollisionObject* m_collisionObject;		//�R���W�����I�u�W�F�N�g�B
	Game* m_game = nullptr;
	P_main_Player* m_player = nullptr;
	GameClear* m_gameclear = nullptr;
	Vector3 m_position;//���W
	Vector3 m_scale = Vector3::One;//�傫��
	Quaternion m_rotation;//��]
	ModelRender m_modelrender;//���f�������_�[
	PhysicsStaticObject m_physicsStaticObject;//�����蔻��
	bool Deletemap = true;
	int GoalLockNum = 0;
};

