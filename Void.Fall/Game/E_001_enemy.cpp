#include "stdafx.h"
#include "E_001_enemy.h"
#include "P_main_Player.h"
///////////////////////////////////////////////////////////
#include <time.h>
#include <stdlib.h>
///////////////////////////////////////////////////////////
#define enemyspeed 100.0f                                //�ړ����x�̐��l
#define enemyserch 700.0f * 700.0f						//�ǐՉ\�͈�
#define enemyserchattack 10.0f * 10.0f                 //�U���\�͈�

bool E_001_enemy::Start()
{

	//�A�j���[�V�����ǂݍ���
	m_animationclips[enAnimationClip_Walk].Load("Assets/animData/Enemy/enemy_001/walkV2.tka");
	m_animationclips[enAnimationClip_Walk].SetLoopFlag(true);

	//���f���ǂݍ���
	m_modelrender = new ModelRender;
	m_modelrender->Init("Assets/modelData/Enemy/enemy_001/enemy_001.tkm", m_animationclips, enAnimationClip_Num);

	//��]
	m_modelrender->SetRotation(m_rotation);
	//���W
	m_modelrender->SetPosition(m_position);
	//�X�P�[��
	m_modelrender->SetScale(m_scale);
	//�L�����R��������
	m_charaCon.Init(
		20.0f,			//���a�B
		100.0f,			//�����B
		m_position		//���W�B
	);

	m_player = FindGO<P_main_Player>("player");
	//�������������B
	srand((unsigned)time(NULL));
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
	return true;
}

void E_001_enemy::Update()
{
	//�ǐՏ���
	Chase();
	//��]����
	Rotation();
	//�U������
	Attack();
	//�A�j���[�V����
	PlayAnimation();
	//�X�e�[�g�J�ڏ���
	ManageState();
	//�`��X�V
	m_modelrender->Update();
}

void E_001_enemy::Chase()
{
	//�ǐՃX�e�[�g�łȂ��Ȃ�A�ǐՏ����͂��Ȃ��B
	if (m_enemystate != enEnemyState_Chase)
	{
		return;
	}
	//�G�l�~�[���ړ�������B
	m_position = m_charaCon.Execute(m_movespeed, g_gameTime->GetFrameDeltaTime());
	//if (m_charaCon.IsOnGround()) {
	//	//�n�ʂɂ����B
	//	//�d�͂�0�ɂ���B
	//	m_movespeed.y = 0.0f;
	//}
	m_movespeed.y = 0.0f;
	Vector3 modelPosition = m_position;
	//������Ƃ������f���̍��W��������B
	modelPosition.y += 2.5f;
	//���W��ݒ肷��B
	m_modelrender->SetPosition(modelPosition);
}

void E_001_enemy::Rotation()
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

	//�v���C���[�̑O�x�N�g�����v�Z����B
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}

const bool E_001_enemy::SearchPlayer() const
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

const bool E_001_enemy::SearchAttackDistance() const
{
	Vector3 diff = m_player->Getposition() - m_position;
	//�v���C���[�ɂ�����x�߂�������.�B

	if (diff.LengthSq() <= enemyserchattack)
	{
		//�v���C���[���˒������ɓ������I
		return true;
	}
	//�v���C���[���˒����O�������B
	return false;
}

void E_001_enemy::Attack()
{
	if (m_enemystate != enEnemyState_Attack)
	{
		return;
	}
	
	if (m_isUnderAttack == true)
	{
		//�����蔻��̂Ƃ�
	}
}

void E_001_enemy::ManageState()
{
	switch (m_enemystate)
	{
	//�ҋ@
	case enEnemyState_Idle:
		//�ҋ@�X�e�[�g�J��
		ProcessIdleStateTransition();
		break;
    //�ǐ�
	case enEnemyState_Chase:
		//�ǐՃX�e�[�g�J��
		ProcessChaseStateTransition();
		break;
	//case enEnemyState_Attack:
	//	//�U���X�e�[�g�J��
	//	break;
	}
}

void E_001_enemy::PlayAnimation()
{
	m_modelrender->SetAnimationSpeed(1.0f);
	switch (m_enemystate)
	{
	//�ҋ@
	//case enEnemyState_Idle:
	//	break;
	//�ړ�
	case enEnemyState_Chase:
		m_modelrender->PlayAnimation(enAnimationClip_Walk, 0.1f);
		break;
	//�U��
	//case enEnemyState_Attack:
	//	break;
	}

}

void E_001_enemy::ProcessCommonStateTransition()
{
	//�e�^�C�}�[���������B
	m_idleTimer = 0.0f;
	m_chaseTimer = 0.0f;

	//�G�l�~�[����v���C���[�Ɍ������x�N�g�����v�Z����B
	Vector3 diff = m_player->Getposition() - m_position;
	//�v���C���[����������B
	if (SearchPlayer() == true)
	{
		//�x�N�g���𐳋K������B
		diff.Normalize();
		//�ړ����x��ݒ肷��B
		m_movespeed = diff * enemyspeed;
		//�U���ł��鋗�����ǂ���
		if (SearchAttackDistance() == true)
		{
			//���݂̃X�e�[�g���U��
			if (m_enemystate == enEnemyState_Attack)
			{
				//�A���Ō������Ȃ��悤��
				//�ǐ�
				m_enemystate = enEnemyState_Chase;
				return;
			}
			//���݂̃X�e�[�g���U���łȂ�
			else
			{
				m_enemystate = enEnemyState_Attack;
				return;
			}
		}
	}
	//�v���C���[���������Ȃ���΁B
	else
	{
		//�ҋ@�X�e�[�g�ɑJ�ڂ���B
		m_enemystate = enEnemyState_Idle;
		return;

	}
}


void E_001_enemy::ProcessIdleStateTransition()
{
	m_idleTimer += g_gameTime->GetFrameDeltaTime();
	//�ҋ@���Ԃ�������x�o�߂�����B
	if (m_idleTimer >= 0.9f)
	{
		//���̃X�e�[�g�֑J�ڂ���B
		ProcessCommonStateTransition();
	}
}

void E_001_enemy::ProcessChaseStateTransition()
{
	//�˒������ɓ�������
	if (SearchAttackDistance() == true)
	{
	//���̃X�e�[�g�ɑJ�ڂ���B
	ProcessCommonStateTransition();
	return;
	}
	m_chaseTimer += g_gameTime->GetFrameDeltaTime();
	//�ǐՎ��Ԃ�������x�o�߂�����B
	if (m_chaseTimer >= 0.8f)
	{
		//���̃X�e�[�g�ɑJ�ڂ���B
		ProcessCommonStateTransition();
	}
}


void E_001_enemy::Render(RenderContext& rc)
{
	m_modelrender->Draw(rc);
}