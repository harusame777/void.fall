#include "stdafx.h"
#include "M_parts_wall.h"

int Imap::boxnum = 0;

bool M_parts_wall::Start()
{
	//ƒ‚ƒfƒ‹“Ç‚İ‚İ
	m_modelrender.Init("Assets/modelData/A_mapmaker/map_parts_wall.tkm");
	//À•Wİ’è
	m_modelrender.SetPosition(m_position);
	//‰ñ“]’lİ’è
	m_modelrender.SetRotation(m_rotation);
	//‘å‚«‚³İ’è
	m_modelrender.SetScale(m_scale);
	m_modelrender.Update();
	SetPhysics();
	boxnum++;
	return true;
}
