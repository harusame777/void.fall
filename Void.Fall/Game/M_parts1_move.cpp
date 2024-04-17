#include "stdafx.h"
#include "M_parts1_move.h"
#define Move1 0
#define Move2 1

bool M_parts1_move::Start()
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

	SetmoveVec();
	m_physicsStaticObject.CreateFromModel(m_modelrender.GetModel(), m_modelrender.GetModel().GetWorldMatrix());
	return true;
}

void M_parts1_move::Update()
{
	//�ړ�����
	Move();
	// �����蔻��̍��W���X�V
	m_physicsStaticObject.GetRigitBody()->SetPositionAndRotation(m_position, m_rotation);
	m_modelrender.Update();
}

void M_parts1_move::Move()
{
	Vector3 diff = m_moveVec[MoveSwitch] - m_position;
	float Dis = diff.Length();
	if (Dis < 20.0f){
		if (MoveSwitch == 0) MoveSwitch = 1;
		else MoveSwitch = 0;
	}
	Vector3 toPosDis = diff;
	toPosDis.Normalize();
	m_position += toPosDis * 2.0f;
	m_modelrender.SetPosition(m_position);
}

void M_parts1_move::Render(RenderContext& rc) {
	//�`�揈��
	m_modelrender.Draw(rc);
}