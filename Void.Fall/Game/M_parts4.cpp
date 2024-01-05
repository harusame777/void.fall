#include "stdafx.h"
#include "M_parts4.h"
#include "M_parts4_sub.h"
#include "Game.h"

bool M_parts4::Start()
{
	//���f���ǂݍ���
	m_modelrender.Init("Assets/modelData/A_mapParts/stage1/mapparts4.tkm");
	//���W�ݒ�
	m_modelrender.SetPosition(m_position);
	//��]�l�ݒ�
	m_modelrender.SetRotation(m_rotation);
	//�傫���ݒ�
	m_modelrender.SetScale(m_scale);
	m_modelrender.Update();
	SetPhysics();


	return true;
}

void M_parts4::mapLockOn()
{
	M_parts4_sub* m_mapparts4_sub = NewGO<M_parts4_sub>(0, "parts4_sub");
	m_mapparts4_sub->Setposition(m_position);
	m_mapparts4_sub->Setrotarion(m_rotation);
	m_mapparts4_sub->Setnum(map_num);
}

void M_parts4::DeleteWall()
{
	DeleteGO(m_mapparts4_sub);
}
