#pragma once
#include "Game.h"
#include "P_main_Player.h"
#include "collision/CollisionObject.h"
class Game;
class P_main_Player;
class IEnemy : public IGameObject
{
public:
	IEnemy() {}
	void SetenemyList()
	{
		m_position.Length();
		m_player->m_enemyPositionList.push_back(&m_position);
		m_player->m_numenemy++;
	}
public:
	P_main_Player* m_player = nullptr;		//�v���C���[
	CharacterController m_charaCon ;        //�L�����R��
	CollisionObject* m_collisionObject;		//�R���W�����I�u�W�F�N�g�B
	Vector3 m_position;						//���W
	Game* m_game = nullptr;
	int m_numenemy = 0;

};

