#pragma once
#include "IItem.h"
class I_G_Item : public IItem
{
public:
	I_G_Item() {};
	~I_G_Item() {};
	bool Start();
	void Update();
	void Render(RenderContext& rc);                    
	const int Rec = 1;
};

