#include "stdafx.h"
#include "Ob_savepoint.h"
#include "P_main_Player.h"
#include "Game.h"

bool Ob_savepoint::Start()
{
	//���f���ǂݍ���
	m_modelrender.Init("Assets/modelData/A_object/savepoint/savepoint.tkm");
	//���W�ݒ�
	m_modelrender.SetPosition(m_position);
	//��]�l�ݒ�
	m_modelrender.SetRotation(m_rotation);
	//�傫���ݒ�
	m_modelrender.SetScale(m_scale);
	m_modelrender.Update();

	//�����蔻��쐬
	m_physicsStaticObject.CreateFromModel(m_modelrender.GetModel(), 
	m_modelrender.GetModel().GetWorldMatrix());
	
	m_game = FindGO<Game>("game");
	m_player = FindGO<P_main_Player>("player");

	SetsaveVec();
	return true;
}


