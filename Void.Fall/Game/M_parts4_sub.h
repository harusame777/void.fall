#pragma once
#include "Imap.h"
class Game;
class M_parts4_sub :public Imap
{
public:
	bool Start();
	void Update();
	void DeleteLock();
	bool GameSumEneNum();
	bool map_delete = false;
	int map_num = 0;
	void Setnum(int num) {
		map_num = num;
	}
	Game* m_game = nullptr;
};
