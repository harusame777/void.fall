#include "stdafx.h"
#include "M_parts2.h"

bool M_parts2::Start()
{
	//ƒ‚ƒfƒ‹“Ç‚Ýž‚Ý
	m_modelrender.Init("Assets/modelData/A_mapParts/stage1/mapparts2.tkm");
	//À•WÝ’è
	m_modelrender.SetPosition(m_position);
	//‰ñ“]’lÝ’è
	m_modelrender.SetRotation(m_rotation);
	//‘å‚«‚³Ý’è
	m_modelrender.SetScale(m_scale);
	m_modelrender.Update();
	PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
	SetPhysics();
	return true;
}

