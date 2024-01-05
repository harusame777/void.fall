#pragma once
#include "Imap.h"
class M_parts4_sub :public Imap
{
public:
	bool Start();
	void DeleteLock();
	int map_num = 0;
	void Setnum(int num) {
		map_num = num;
	}
};
