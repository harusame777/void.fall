#include "stdafx.h"
#include "M_parts_start.h"

bool M_parts_start::Start()
{
	//���f���ǂݍ���
	m_modelrender.Init("Assets/modelData/A_mapParts/stage1/mapparts1.tkm");
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
