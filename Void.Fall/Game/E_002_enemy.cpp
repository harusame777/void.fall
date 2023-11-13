#include "stdafx.h"
#include "E_002_enemy.h"
#include "IEnemy.h"
#include "Game.h"
#include "collision/CollisionObject.h"
#include "P_main_Player.h"
#define enemyspeed 100.0f                               //�ړ����x�̐��l
#define enemyserch 700.0f * 700.0f						//�ǐՉ\�͈�

namespace
{
	const Vector3 corre = { 0.0f,60.0f,0.0f };//�ʒu�C��
}

bool E_002_enemy::Start()
{

	//�A�j���[�V�����ǂݍ���
	m_animationclips[enAnimationClip_Idle].Load("Assets/animData/Enemy/enemy_002/idle.tka");
	m_animationclips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationclips[enAnimationClip_Chase].Load("Assets/animData/Enemy/enemy_002/walk.tka");
	m_animationclips[enAnimationClip_Chase].SetLoopFlag(true);

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
	//�ǐՏ���
	Chase();
	//��]����
	Rotation();
	//�A�j���[�V����
	PlayAnimation();
	//�X�e�[�g�J�ڏ���
	ManageState();
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
	case enEnemyState_Chase:
		m_modelrender->PlayAnimation(enAnimationClip_Chase, 0.1f);
		break;
	}
}

void E_002_enemy::ManageState()
{
	switch (m_enemystate)
	{
	case E_002_enemy::enEnemyState_Idle:
		ProcessIdleStateTransition();
		break;
	case E_002_enemy::enEnemyState_Chase:
		ProcessChaseStateTransition();
		break;
	}
}

void E_002_enemy::ProcessCommonStateTransition()
{
	Vector3 diff = m_player->Getposition() - m_position;

	if (SearchPlayer() == true)
	{
		//�x�N�g���𐳋K������B
		diff.Normalize();
		//�ړ����x��ݒ肷��B
		m_movespeed = diff * enemyspeed;
		m_enemystate = enEnemyState_Chase;
	}
	else
	{
		m_enemystate = enEnemyState_Idle;
	}
}

void E_002_enemy::ProcessIdleStateTransition()
{
	ProcessCommonStateTransition();
}

void E_002_enemy::ProcessChaseStateTransition()
{
	ProcessCommonStateTransition();
}

void E_002_enemy::Chase()
{
	//�ǐՃX�e�[�g�łȂ��Ȃ�A�ǐՏ����͂��Ȃ��B
	if (m_enemystate != enEnemyState_Chase)
	{
		return;
	}

	//�G�l�~�[���ړ�������B
	m_position = m_charaCon.Execute(m_movespeed, g_gameTime->GetFrameDeltaTime());
	if (m_charaCon.IsOnGround()) {
		//�n�ʂɂ����B
		//�d�͂�0�ɂ���B
		m_movespeed.y = 0.0f;
	}
	Vector3 modelPosition = m_position;
	//������Ƃ������f���̍��W��������B
	modelPosition.y += 2.5f;
	//���W��ݒ肷��B
	m_modelrender->SetPosition(modelPosition);
}

void E_002_enemy::Rotation()
{
	if (fabsf(m_movespeed.x) < 0.001f
		&& fabsf(m_movespeed.z) < 0.001f) {
		//m_moveSpeed.x��m_moveSpeed.z�̐�Βl���Ƃ���0.001�ȉ��Ƃ������Ƃ�
		//���̃t���[���ł̓L�����͈ړ����Ă��Ȃ��̂Ő��񂷂�K�v�͂Ȃ��B
		return;
	}
	//atan2��tan�Ƃ̒l���p�x(���W�A���P��)�ɕϊ����Ă����֐��B
	//m_moveSpeed.x / m_moveSpeed.z�̌��ʂ�tan�ƂɂȂ�B
	//atan2���g�p���āA�p�x�����߂Ă���B
	//���ꂪ��]�p�x�ɂȂ�B
	float angle = atan2(-m_movespeed.x, m_movespeed.z);
	//atan���Ԃ��Ă���p�x�̓��W�A���P�ʂȂ̂�
	//SetRotationDeg�ł͂Ȃ�SetRotation���g�p����B
	m_rotation.SetRotationY(-angle);

	//��]��ݒ肷��B
	m_modelrender->SetRotation(m_rotation);
	m_collisionObject->SetRotation(m_rotation);
	m_collisionObject->SetPosition(m_position + corre);

	//�v���C���[�̑O�x�N�g�����v�Z����B
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}

const bool E_002_enemy::SearchPlayer() const
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

void E_002_enemy::Render(RenderContext& rc)
{
	m_modelrender->Draw(rc);
}


