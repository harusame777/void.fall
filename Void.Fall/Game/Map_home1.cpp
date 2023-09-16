#include "stdafx.h"
#include "Map_home1.h"

bool Map_home1::Start()
{
	//���f���ǂݍ���
	m_modelrender.Init("Assets/modelData/A_testmap/testmap.tkm");
	//���W�ݒ�
	m_modelrender.SetPosition(m_position);
	//��]�l�ݒ�
	m_modelrender.SetRotation(m_rotation);
	//�傫���ݒ�
	m_modelrender.SetScale(m_scale);
	m_modelrender.Update();
	//�����蔻��쐬
	m_physicsStaticObject.CreateFromModel(m_modelrender.GetModel(), m_modelrender.GetModel().GetWorldMatrix());
	//�����蔻�����������B
	PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
	return true;
}

void Map_home1::Update()
{
	//�`��X�V
	m_modelrender.Update();
}

void Map_home1::Render(RenderContext& rc)
{
	//�`�揈��
	m_modelrender.Draw(rc);
}