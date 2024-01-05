#pragma once
#include "Imap.h"
class M_parts4_sub;
class Game;
class M_parts4 :public Imap
{
public:
	bool Start();
	void mapLockOn();
	void DeleteWall();
	M_parts4_sub* m_mapparts4_sub = nullptr;
	int map_num = 0;
	void Setnum(int num){
		map_num = num;
	}
	Game* m_game = nullptr;
};

