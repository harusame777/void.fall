#include "stdafx.h"
#include "M_parts4_sub.h"

bool M_parts4_sub::Start()
{
	//ƒ‚ƒfƒ‹“Ç‚İ‚İ
	m_modelrender.Init("Assets/modelData/A_mapParts/stage1/mapparts4_sub.tkm");
	//À•Wİ’è
	m_modelrender.SetPosition(m_position);
	//‰ñ“]’lİ’è
	m_modelrender.SetRotation(m_rotation);
	//‘å‚«‚³İ’è
	m_modelrender.SetScale(m_scale);
	m_modelrender.Update();
	SetPhysics();
	return true;
}

void M_parts4_sub::DeleteLock()
{
	DeleteGO(this);
}