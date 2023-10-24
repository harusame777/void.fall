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
	P_main_Player* m_player = nullptr;		//プレイヤー
	CharacterController m_charaCon ;        //キャラコン
	CollisionObject* m_collisionObject;		//コリジョンオブジェクト。
};

