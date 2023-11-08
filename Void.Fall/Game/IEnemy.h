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
	P_main_Player* m_player = nullptr;		//プレイヤー
	CharacterController m_charaCon ;        //キャラコン
	CollisionObject* m_collisionObject;		//コリジョンオブジェクト。
	Vector3 m_position;						//座標
	Game* m_game = nullptr;
	int m_numenemy = 0;

};

