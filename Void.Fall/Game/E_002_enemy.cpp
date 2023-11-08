#include "stdafx.h"
#include "E_002_enemy.h"
#include "IEnemy.h"
#include "Game.h"
#include "collision/CollisionObject.h"
#include "P_main_Player.h"

namespace
{
	const Vector3 corre = { 0.0f,60.0f,0.0f };//�ʒu�C��
}

bool E_002_enemy::Start()
{

	//�A�j���[�V�����ǂݍ���
	m_animationclips[enAnimationClip_Idle].Load("Assets/animData/Enemy/enemy_002/idle.tka");
	m_animationclips[enAnimationClip_Idle].SetLoopFlag(true);

	//���f���ǂݍ���
	m_modelrender = new ModelRender;
	m_modelrender->Init("Assets/modelData/Enemy/enemy_002/RE_enemy_002.tkm", m_animationclips, enAnimationClip_Num);

	//��]
	m_modelrender->SetRotation(m_rotation);
	//���W
	m_modelrender->SetPosition(m_position);
	//�X�P�[��
	m_modelrender->SetScale(m_scale);
	//�L�����R��������
	m_charaCon.Init(20.0f, 100.0f, m_position);

	//�R���W�����I�u�W�F�N�g���쐬����B
	m_collisionObject = NewGO<CollisionObject>(0);
	//����̃R���W�������쐬����B
	m_collisionObject->CreateSphere(m_position, Quaternion::Identity, 60.0f * m_scale.z);
	m_collisionObject->SetName("enemy_col");
	m_collisionObject->SetPosition(m_position + corre);
	//�R���W�����I�u�W�F�N�g�������ō폜����Ȃ��悤�ɂ���B
	m_collisionObject->SetIsEnableAutoDelete(false);

	m_player = FindGO<P_main_Player>("player");
	SetenemyList();

	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
	return true;
}

void E_002_enemy::Update()
{
	//�A�j���[�V����
	PlayAnimation();
	//�`��X�V
	m_modelrender->Update();
}

void E_002_enemy::PlayAnimation()
{
	m_modelrender->SetAnimationSpeed(1.0f);
	switch (m_enemystate)
	{
		//�ҋ@
	case enEnemyState_Idle:
		m_modelrender->PlayAnimation(enAnimationClip_Idle, 0.1f);
		break;
	}
}
void E_002_enemy::Render(RenderContext& rc)
{
	m_modelrender->Draw(rc);
}


