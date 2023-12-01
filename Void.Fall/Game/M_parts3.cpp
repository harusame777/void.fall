#include "stdafx.h"
#include "M_parts3.h"


bool M_parts3::Start()
{
	//モデル読み込み
	m_modelrender.Init("Assets/modelData/A_mapParts/stage1/mapparts3.tkm");
	//座標設定
	m_modelrender.SetPosition(m_position);
	//回転値設定
	m_modelrender.SetRotation(m_rotation);
	//大きさ設定
	m_modelrender.SetScale(m_scale);
	m_modelrender.Update();
	SetPhysics();
	return true;
}

