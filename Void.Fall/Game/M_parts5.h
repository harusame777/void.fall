#pragma once
#include "Imap.h"
class P_main_Player;
class Game;
class GameClear;
class M_parts5 : public Imap
{
public:
	bool Start();
	void Update();
	void Collision();										//本体の当たり判定
	void Lock();
	void Render(RenderContext& rc);
	CollisionObject* m_collisionObject;		//コリジョンオブジェクト。
	Game* m_game = nullptr;
	P_main_Player* m_player = nullptr;
	GameClear* m_gameclear = nullptr;
};

