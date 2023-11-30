#pragma once
#include "IItem.h"
#include "graphics/effect/EffectEmitter.h"
class I_G_Item : public IItem
{
public:
	I_G_Item() {};
	~I_G_Item() {};
	bool Start();
	void Update();
	void EffectDraw();
	const int Rec = 1;
};

