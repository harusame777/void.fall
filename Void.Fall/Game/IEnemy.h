#pragma once
#include "Game.h"
#include "P_main_Player.h"
#include "collision/CollisionObject.h"
class Game;
class P_main_Player;
class IEnemy : public IGameObject
{
public:
	IEnemy() {
		//enemyvec[m_game->m_numenemy] = &m_position;
	};
	//void Vecenemynew(){
		//enemyvec[m_game->m_numenemy] = new Vector3;
	//}
	virtual ~IEnemy(){};
public:
	P_main_Player* m_player = nullptr;		//�v���C���[
	CharacterController m_charaCon ;        //�L�����R��
	CollisionObject* m_collisionObject;		//�R���W�����I�u�W�F�N�g�B
	Vector3 m_position;						//���W
	Game* m_game;

};

