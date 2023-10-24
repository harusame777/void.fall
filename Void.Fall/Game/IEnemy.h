#pragma once
#include "P_main_Player.h"
#include "collision/CollisionObject.h"
class P_main_Player;
class IEnemy : public IGameObject
{
public:
	IEnemy() {};
	virtual ~IEnemy(){};
public:
	P_main_Player* m_player = nullptr;		//�v���C���[
	CharacterController m_charaCon ;        //�L�����R��
	CollisionObject* m_collisionObject;		//�R���W�����I�u�W�F�N�g�B
};

