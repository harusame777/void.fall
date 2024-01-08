#include "stdafx.h"
#include "Ob_savepoint.h"
#include "P_main_Player.h"
#include "Game.h"

bool Ob_savepoint::Start()
{
	//ƒ‚ƒfƒ‹“Ç‚İ‚İ
	m_modelrender.Init("Assets/modelData/A_object/savepoint/savepoint.tkm");
	//À•Wİ’è
	m_modelrender.SetPosition(m_position);
	//‰ñ“]’lİ’è
	m_modelrender.SetRotation(m_rotation);
	//‘å‚«‚³İ’è
	m_modelrender.SetScale(m_scale);
	m_modelrender.Update();

	//“–‚½‚è”»’èì¬
	m_physicsStaticObject.CreateFromModel(m_modelrender.GetModel(), 
	m_modelrender.GetModel().GetWorldMatrix());
	
	m_game = FindGO<Game>("game");
	m_player = FindGO<P_main_Player>("player");

	SetsaveVec();
	return true;
}


