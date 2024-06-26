#include "stdafx.h"
#include "M_parts_wall.h"

int Imap::boxnum = 0;

bool M_parts_wall::Start()
{
	//モデル読み込み
	m_modelrender.Init("Assets/modelData/A_mapmaker/map_parts_wall.tkm");
	//座標設定
	m_modelrender.SetPosition(m_position);
	//回転値設定
	m_modelrender.SetRotation(m_rotation);
	//大きさ設定
	m_modelrender.SetScale(m_scale);
	m_modelrender.Update();
	SetPhysics();
	boxnum++;
	return true;
}
