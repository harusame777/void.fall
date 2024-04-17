#pragma once
#include "Imap.h"
#define MoveDis 1000.0f

class M_parts1_move : public Imap

{
public:
	//ŠÖ”éŒ¾
	bool Start();
	void Update();
	void Move();
	void Render(RenderContext& rc);
	void SetmoveVec(){
		Vector3 corre1 = { 0.0f,0.0f,MoveDis };
		m_moveVec[0] = m_position + corre1;
		m_moveVec[1] = m_position;
	}
	int MoveSwitch = 0;
	Vector3 m_moveVec[2];
	ModelRender m_modelrender;//ƒ‚ƒfƒ‹ƒŒƒ“ƒ_[
	PhysicsStaticObject m_physicsStaticObject;
};

