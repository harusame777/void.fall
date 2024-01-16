#include "stdafx.h"
#include "E_002_enemy.h"
#include "IEnemy.h"
#include "Game.h"
#include "collision/CollisionObject.h"
#include "P_main_Player.h"
#define enemyspeed 150.0f                               //�ړ����x�̐��l
#define enemyattackspeed 300.0f                         //�U�����ړ����x�̐��l
#define enemyserch 700.0f * 700.0f						//�ǐՉ\�͈�

namespace
{
	const Vector3 corre1 = { 0.0f,60.0f,0.0f };//�ʒu�C��
	const Vector3 corre2 = { 2.0f,2.0f,2.0f };
}

bool E_002_enemy::Start()
{

	//�A�j���[�V�����ǂݍ���
	m_animationclips[enAnimationClip_Idle].Load("Assets/animData/Enemy/enemy_002/idle.tka");
	m_animationclips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationclips[enAnimationClip_Chase].Load("Assets/animData/Enemy/enemy_002/walk.tka");
	m_animationclips[enAnimationClip_Chase].SetLoopFlag(true);
	m_animationclips[enAnimationClip_Attack].Load("Assets/animData/Enemy/enemy_002/attack.tka");
	m_animationclips[enAnimationClip_Attack].SetLoopFlag(true);
	m_animationclips[enAnimationClip_ReceiveDamage].Load("Assets/animData/Enemy/enemy_002/receivedamage.tka");
	m_animationclips[enAnimationClip_ReceiveDamage].SetLoopFlag(false);
	m_animationclips[enAnimationClip_Down].Load("Assets/animData/Enemy/enemy_002/down.tka");
	m_animationclips[enAnimationClip_Down].SetLoopFlag(false);


	//���f���ǂݍ���
	m_modelrender.Init("Assets/modelData/Enemy/enemy_002/RE_enemy_002.tkm", m_animationclips, enAnimationClip_Num);

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
	Sheldeffect();

	return true;
}

void E_002_enemy::Update()
{
	//�ǐՏ���
	Chase();
	//��]����
	Rotation();
	//�U������
	Attack();
	//�����蔻�菈��
	Collision();
	//�A�j���[�V����
	PlayAnimation();
	//�X�e�[�g�J�ڏ���
	ManageState();
	//�`��X�V
	m_modelrender.Update();
}

void E_002_enemy::PlayAnimation()
{
	m_modelrender.SetAnimationSpeed(1.0f);
	switch (m_enemystate)
	{
		//�ҋ@
	case enEnemyState_Idle:
		m_modelrender.PlayAnimation(enAnimationClip_Idle, 0.1f);
		break;
	case enEnemyState_Chase:
		m_modelrender.PlayAnimation(enAnimationClip_Chase, 0.1f);
		break;
	case enEnemyState_Attack:
		m_modelrender.PlayAnimation(enAnimationClip_Attack, 0.1f);
		break;
	case enEnemyState_ReceiveDamage:
		m_modelrender.PlayAnimation(enAnimationClip_ReceiveDamage, 0.1f);
		break;
	case enEnemyState_Down:
		m_modelrender.PlayAnimation(enAnimationClip_ReceiveDamage, 0.1f);
		break;

	}
}

void E_002_enemy::ManageState()
{
	switch (m_enemystate)
	{
	case enEnemyState_Idle:
		ProcessIdleStateTransition();
		break;
	case enEnemyState_Chase:
		ProcessChaseStateTransition();
		break;
	case enEnemyState_Attack:
		ProcessAttackStateTransition();
		break;
	case enEnemyState_ReceiveDamage:
		ProcessReceiveDamageStateTransition();
		break;
	case enEnemyState_Down:
		ProcessDownStateTransition();
		break;
	}
}

void E_002_enemy::ProcessCommonStateTransition()
{
	if (m_attackcooltimer > 0){
		m_attackcooltimer -= g_gameTime->GetFrameDeltaTime();
		return;
	}
	if (m_enemystate == enEnemyState_Attack){
		return;
	}
	Vector3 diff = m_player->Getposition() - m_position;

	//�x�N�g���𐳋K������B
	diff.Normalize();
	//�ړ����x��ݒ肷��B
	m_movespeed = diff * enemyspeed;
	if (SearchPlayer())
	{
		if (SearchAttackDistance()){
			if (m_attackTimer < 0.0f) {
				m_enemystate = enEnemyState_Attack;
				m_attackTimer = attacktime;
				m_attackGotimer = attackGotime;
				m_movespeed = diff * enemyattackspeed;
				return;
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
	}
	else
	{
		m_attackTimer = attacktime;
		m_enemystate = enEnemyState_Idle;
		return;
	}
}

void E_002_enemy::ProcessIdleStateTransition()
{
	ProcessCommonStateTransition();
}

void E_002_enemy::ProcessChaseStateTransition()
{
	//�˒������ɓ�������
	if (SearchAttackDistance() == true)
	{
		//���̃X�e�[�g�ɑJ�ڂ���B
		m_attackTimer -= g_gameTime->GetFrameDeltaTime();
		ProcessCommonStateTransition();
		return;
	}
	ProcessCommonStateTransition();
}

void E_002_enemy::ProcessAttackStateTransition()
{
	if (m_attackGotimer >= 0){
		ProcessCommonStateTransition();
	}
	else
	{
		m_enemystate = enEnemyState_Idle;
		DeleteGO(m_attackcoll);
		m_attackcooltimer = attackcooltime;
		ProcessCommonStateTransition();
	}
}

void E_002_enemy::ProcessReceiveDamageStateTransition()
{
	//��_���[�W�A�j���[�V�����̍Đ����I�������B
	if (m_modelrender.IsPlayingAnimation() == false)
	{
		ProcessCommonStateTransition();
	}
}

void E_002_enemy::ProcessDownStateTransition()
{
	//��_���[�W�A�j���[�V�����̍Đ����I�������B
	if (m_modelrender.IsPlayingAnimation() == false)
	{
		SoundSource* se = NewGO<SoundSource>(11);
		se = NewGO<SoundSource>(11);
		se->Init(11);
		se->Play(false);
		ItemDrop();
		DeleteGoEnemyList();
		DeleteGO(m_collisionObject);//��������
		DeleteGO(this);
	}
}

void E_002_enemy::Chase()
{
	//�ǐՃX�e�[�g�łȂ��Ȃ�A�ǐՏ����͂��Ȃ��B
	if (IsAttack()){
		return;
	}
	if (m_attackGotimer > 0){
		m_attackGotimer -= g_gameTime->GetFrameDeltaTime();
	}
	//�G�l�~�[���ړ�������B
	m_position = m_charaCon.Execute(m_movespeed, g_gameTime->GetFrameDeltaTime());
	if (m_charaCon.IsOnGround()) {
		//�n�ʂɂ����B
		//�d�͂�0�ɂ���B
		m_movespeed.y = 0.0f;
	}
	Vector3 modelPosition = m_position;
	m_collisionObject->SetRotation(m_rotation);
	m_collisionObject->SetPosition(m_position + corre1);
	//������Ƃ������f���̍��W��������B
	modelPosition.y += 2.5f;
	//���W��ݒ肷��B
	m_modelrender.SetPosition(modelPosition);
}

void E_002_enemy::Rotation()
{
	if (m_enemystate == enEnemyState_Attack){
		return;
	}
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
	m_modelrender.SetRotation(m_rotation);

	//�v���C���[�̑O�x�N�g�����v�Z����B
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}

void E_002_enemy::Attack()
{
	if (m_enemystate == enEnemyState_Attack){
		//�U�������蔻��p�̃R���W�����I�u�W�F�N�g���쐬����B
		m_attackcoll = NewGO<CollisionObject>(0);
		//����̃R���W�������쐬����B
		m_attackcoll->CreateSphere(m_position, Quaternion::Identity, 50.0f * m_scale.z);
		m_attackcoll->SetName("enemy_attack");
		Vector3 position = m_position + m_forward * 40.0f;
		m_attackcoll->SetPosition(position + corre1);
	}
}

void E_002_enemy::Collision()
{
	//��_���[�W�A���邢�̓_�E���X�e�[�g�̎��́B
//�����蔻�菈���͂��Ȃ��B
	if (m_enemystate == enEnemyState_ReceiveDamage ||
		m_enemystate == enEnemyState_Down)
	{
		return;
	}
	//�v���C���[�̍U���p�̃R���W�������擾����B
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_attack");
	//�R���W�����̔z���for���ŉ񂷁B
	for (auto collision : collisions)
	{
		//�R���W�����ƃL�����R�����Փ˂�����B
		if (collision->IsHit(m_collisionObject))
		{
			SoundSource* se = NewGO<SoundSource>(5);
			se = NewGO<SoundSource>(5);
			se->Init(5);
			se->Play(false);
			if (m_sh > 0) {
				m_sh--;
				//��_���[�W�X�e�[�g�ɑJ�ڂ���B
				m_enemystate = enEnemyState_ReceiveDamage;
				return;
			}
			else {
				//HP��1���炷�B
				m_hp -= 1;
				//HP��0�ɂȂ�����B
				if (m_hp == 0) {
					//�_�E���X�e�[�g�ɑJ�ڂ���B
					m_enemystate = enEnemyState_Down;
				}
				else {
					//��_���[�W�X�e�[�g�ɑJ�ڂ���B
					m_enemystate = enEnemyState_ReceiveDamage;
				}
			}
		}
	}
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

const bool E_002_enemy::SearchAttackDistance() const
{
	Vector3 diff = m_player->Getposition() - m_position;
	//�v���C���[�ɂ�����x�߂�������.�B

	if (diff.LengthSq() <= enemyserch)
	{
		//�v���C���[���˒������ɓ������I
		return true;
	}
	//�v���C���[���˒����O�������B
	return false;
}

void E_002_enemy::Render(RenderContext& rc)
{
	m_modelrender.Draw(rc);
	if (m_enemysheld_type == SheldEnemy) {
		if (m_effectSH->IsPlay() == false) {
			Shplay();
		}
	}
}


