#include "stdafx.h"
#include "E_003_enemy.h"
#include "P_main_Player.h"
#define enemyspeed 50.0f
#define enemyserch 700.0f * 700.0f						//�ǐՉ\�͈�
#define enemyserchAttackN 200.0f * 200.0f				//�U���\�͈�(��)
#define enemyserchAttackF 500.0f * 500.0f				//�U���\�͈�(��)

namespace
{
	const Vector3 corre1 = { 0.0f,60.0f,0.0f };//�ʒu�C��
}

bool E_003_enemy::Start()
{
	m_animationclips[enAnimationClip_Idle].Load("Assets/animData/Enemy/enemy_003/idle.tka");
	m_animationclips[enAnimationClip_Idle].SetLoopFlag(true);

	m_modelrender.Init("Assets/modelData/Enemy/enemy_003/RE_enemy_003.tkm"
		, m_animationclips, enAnimationClip_Num);
	//��]
	m_modelrender.SetRotation(m_rotation);
	//���W
	m_modelrender.SetPosition(m_position);
	//�X�P�[��
	m_modelrender.SetScale(m_scale);
	//�L�����R��������
	m_charaCon.Init(20.0f, 100.0f, m_position);

	//�R���W�����I�u�W�F�N�g���쐬����B
	m_collisionObject = NewGO<CollisionObject>(0);
	//����̃R���W�������쐬����B
	m_collisionObject->CreateSphere(m_position, Quaternion::Identity, 60.0f * m_scale.z);
	m_collisionObject->SetName("enemy_col");
	m_collisionObject->SetPosition(m_position + corre1);
	//�R���W�����I�u�W�F�N�g�������ō폜����Ȃ��悤�ɂ���B
	m_collisionObject->SetIsEnableAutoDelete(false);

	m_player = FindGO<P_main_Player>("player");
	m_game = FindGO<Game>("game");

	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);

	EnemyGoEffect();

	return true;
}

void E_003_enemy::Update()
{
	//�A�j���[�V����
	PlayAnimation();
	//�X�e�[�g�J�ڏ���
	ManageState();
	//�`��X�V
	m_modelrender.Update();
}

void E_003_enemy::PlayAnimation()
{
	m_modelrender.SetAnimationSpeed(1.0f);
	switch (m_enemystate)
	{
	case enEnemyState_Idle:
		m_modelrender.PlayAnimation(enAnimationClip_Idle, 0.1f);
		break;
	}
}

void E_003_enemy::ManageState()
{
	switch (m_enemystate)
	{
	case E_003_enemy::enEnemyState_Idle:
		ProcessIdleStateTransition();
		break;
	}
}

void E_003_enemy::ProcessCommonStateTransition()
{
	Vector3 diff = m_player->Getposition() - m_position;

	//�x�N�g���𐳋K������B
	diff.Normalize();
	//�ړ����x��ݒ肷��B
	m_movespeed = diff * enemyspeed;
	if (SearchPlayer()){
		if (SearchAttackDistance(en_attackN))
		{

		}
		if (m_attackTimer < 0.0f) {
			m_enemystate = enEnemyState_Attack;
			m_attackTimer = attacktime;
		}
		if (m_enemystate == enEnemyState_Attack) {
			m_enemystate = enEnemyState_Chase;
			return;
		}
		else {
			m_enemystate = enEnemyState_Chase;
			return;
		}
	}
	else{
		m_attackTimer = attacktime;
		m_enemystate = enEnemyState_Idle;
		return;
	}
}

const bool E_003_enemy::SearchPlayer() const
{
	Vector3 diff = m_player->Getposition() - m_position;

	//�v���C���[�ɂ�����x�߂�������.�B
	if (diff.LengthSq() <= enemyserch)
	{
		return true;
	}
	//�v���C���[���������Ȃ������B
	return false;
}

const bool E_003_enemy::SearchAttackDistance() const
{
	Vector3 diff = m_player->Getposition() - m_position;

	//�v���C���[�ɂ�����x�߂�������.�B
	if (diff.LengthSq() <= enemyserch)
	{
		return true;
	}
	//�v���C���[���������Ȃ������B
	return false;
}

void E_003_enemy::ProcessIdleStateTransition()
{
	ProcessCommonStateTransition();
}

void E_003_enemy::Render(RenderContext& rc)
{
	m_modelrender.Draw(rc);
}