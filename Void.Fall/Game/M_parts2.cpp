#include "stdafx.h"
#include "M_parts2.h"

bool M_parts2::Start()
{
	//���f���ǂݍ���
	m_modelrender.Init("Assets/modelData/A_mapParts/stage1/mapparts2.tkm");
	//���W�ݒ�
	m_modelrender.SetPosition(m_position);
	//��]�l�ݒ�
	m_modelrender.SetRotation(m_rotation);
	//�傫���ݒ�
	m_modelrender.SetScale(m_scale);
	m_modelrender.Update();
	PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
	SetPhysics();
	return true;
}

