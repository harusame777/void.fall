#include "stdafx.h"
#include "Ob_savepoint.h"
#include "P_main_Player.h"
#include "Game.h"

bool Ob_savepoint::Start()
{
	//モデル読み込み
	m_modelrender.Init("Assets/modelData/A_object/savepoint/savepoint.tkm");
	//座標設定
	m_modelrender.SetPosition(m_position);
	//回転値設定
	m_modelrender.SetRotation(m_rotation);
	//大きさ設定
	m_modelrender.SetScale(m_scale);
	m_modelrender.Update();

	//当たり判定作成
	m_physicsStaticObject.CreateFromModel(m_modelrender.GetModel(), 
	m_modelrender.GetModel().GetWorldMatrix());
	
	m_game = FindGO<Game>("game");
	m_player = FindGO<P_main_Player>("player");

	SetsaveVec();
	return true;
}


