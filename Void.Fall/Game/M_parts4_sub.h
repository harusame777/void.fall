#pragma once
#include "Imap.h"
class M_parts4_sub :public Imap
{
public:
	bool Start();
	void DeleteLock();
};
