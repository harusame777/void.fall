#include "stdafx.h"
#include "E_001_enemy.h"
#include "P_main_Player.h"
#include "collision/CollisionObject.h"
#include "B_homingbullet.h"
#include "Game.h"
#include "graphics/effect/EffectEmitter.h"
///////////////////////////////////////////////////////////
#include <time.h>
#include <stdlib.h>
///////////////////////////////////////////////////////////
#define enemyspeed 100.0f                               //�ړ����x�̐��l
#define enemyserch 700.0f * 700.0f						//�ǐՉ\�͈�
#define enemyserchnear 200.0f * 200.0f					//�ǐՉ\�͈�(�ߐ�)
#define attacktime 5.0f									//�A�^�b�N�^�C�}�[
namespace
{
	const Vector3 corre1 = { 0.0f,60.0f,0.0f };//�ʒu�C���{�̓����蔻��
	const Vector3 corre2 = { 0.0f,80.0f,10.0f };//�ʒu�C���e�۔����ʒu
}

bool E_001_enemy::Start()
{

	//�A�j���[�V�����ǂݍ���
	m_animationclips[enAnimationClip_Idle].Load("Assets/animData/Enemy/enemy_001/idle.tka");
	m_animationclips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationclips[enAnimationClip_Chase].Load("Assets/animData/Enemy/enemy_001/chase.tka");
	m_animationclips[enAnimationClip_Chase].SetLoopFlag(true);
	//enAnimationClip_Attack:�A�j���[�V�����L�[name(attack_point)
	m_animationclips[enAnimationClip_Attack].Load("Assets/animData/Enemy/enemy_001/attack.tka");
	m_animationclips[enAnimationClip_Attack].SetLoopFlag(false);
	//enAnimationClip_AttackNear:�A�j���[�V�����L�[name(attack_start)
	//enAnimationClip_AttackNear:�A�j���[�V�����L�[name(attack_end)
	//bone���(sub1)
	m_animationclips[enAnimationClip_AttackNear].Load("Assets/animData/Enemy/enemy_001/attacknear.tka");
	m_animationclips[enAnimationClip_AttackNear].SetLoopFlag(false);
	m_animationclips[enAnimationClip_ReceiveDamage].Load("Assets/animData/Enemy/enemy_001/receivedamage.tka");
	m_animationclips[enAnimationClip_ReceiveDamage].SetLoopFlag(false);
	m_animationclips[enAnimationClip_Down].Load("Assets/animData/Enemy/enemy_001/down.tka");
	m_animationclips[enAnimationClip_ReceiveDamage].SetLoopFlag(false);

	//�G�t�F�N�g�ǂݍ���

	//���f���ǂݍ���
	m_modelrender.Init("Assets/modelData/Enemy/enemy_001/RE_enemy_001.tkm",m_animationclips, enAnimationClip_Num);

	//���̃{�[����ID���擾����B
	m_attacknearboneID = m_modelrender.FindBoneID(L"sub1");

	//�A�j���[�V�����C�x���g�p�֐��ݒ�
	m_modelrender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	//��]
	m_modelrender.SetRotation(m_rotation);
	//���W
	m_modelrender.SetPosition(m_position);
	//�X�P�[��
	m_modelrender.SetScale(m_scale);
	//�L�����R��������
	m_charaCon.Init(20.0f,100.0f,m_position);

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

void E_001_enemy::Update()
{
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
	m_modelrender.SetRotation(m_rotation);
	m_collisionObject->SetRotation(m_rotation);
	m_collisionObject->SetPosition(m_position + corre1);

	//�v���C���[�̑O�x�N�g�����v�Z����B
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}

void E_001_enemy::Collision()
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
			if (m_sh > 0){
				m_sh--;
				//��_���[�W�X�e�[�g�ɑJ�ڂ���B
				m_enemystate = enEnemyState_ReceiveDamage;
				return;
			}
			else
			{
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

	if (diff.LengthSq() <= enemyserch)
	{
		//�v���C���[���˒������ɓ������I
		return true;
	}
	//�v���C���[���˒����O�������B
	return false;
}

const bool E_001_enemy::SearchAttackDistanceNear() const
{
	Vector3 diff2 = m_player->Getposition() - m_position;
	//�v���C���[�ɂ�����x�߂�������.�B

	if (diff2.LengthSq() <= enemyserchnear)
	{
		//�v���C���[���˒������ɓ������I
		return true;
	}
	//�v���C���[���˒����O�������B
	return false;
}

void E_001_enemy::Attack()
{
	if (m_enemystate != enEnemyState_AttackNear)
	{
		return;
	}
	
	if (m_isUnderAttack == true)
	{
		//�����蔻��̂Ƃ�
		MakeAttackCollision();
	}
}

void E_001_enemy::MakeAttackCollision()
{
	//�U�������蔻��p�̃R���W�����I�u�W�F�N�g���쐬����B
	auto collisionObject = NewGO<CollisionObject>(0);
	//���̃{�[���̃��[���h�s����擾����B
	Matrix matrix = m_modelrender.GetBone(m_attacknearboneID)->GetWorldMatrix();
	//�{�b�N�X��̃R���W�������쐬����B
	collisionObject->CreateBox(m_position, Quaternion::Identity, Vector3(100.0f, 10.0f, 10.0f));
	collisionObject->SetWorldMatrix(matrix);
	collisionObject->SetName("enemy_attack");
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
	case enEnemyState_Attack:
	case enEnemyState_AttackNear:
		//�U���X�e�[�g�J��
		ProcessAttackStateTransition();
		break;
	case enEnemyState_ReceiveDamage:
		//��_���X�e�[�g�J��
		ProcessReceiveDamageStateTransition();
		break;
	case enEnemyState_Down:
		//�_�E���X�e�[�g�J��
		ProcessDownStateTransition();
		break;
	}
}

void E_001_enemy::PlayAnimation()
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
	case enEnemyState_AttackNear:
		m_modelrender.PlayAnimation(enAnimationClip_AttackNear, 0.1f);
		break;
	case enEnemyState_ReceiveDamage:
		m_modelrender.PlayAnimation(enAnimationClip_ReceiveDamage, 0.1f);
		break;
	case enEnemyState_Down :
		m_modelrender.PlayAnimation(enAnimationClip_Down, 0.1f);
		break;
	}
}

void E_001_enemy::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	(void)clipName;
	//�L�[�̖��O���F�X
	if (wcscmp(eventName, L"attack_start") == 0){
		m_isUnderAttack = true;
	}
	else if (wcscmp(eventName, L"attack_end") == 0){
		m_isUnderAttack = false;
	}
	else if (wcscmp(eventName, L"attack_point") == 0) {
		SoundSource* se = NewGO<SoundSource>(7);
		se = NewGO<SoundSource>(7);
		se->Init(7);
		se->Play(false);
		auto bullet = NewGO<B_homingbullet>(0);
		bullet->SetPosition(m_position + corre2);
		bullet->SetVelocity(m_forward * 10);
		bullet->SetEnShooter(B_homingbullet::enShooter_Enemy);
		m_effect = NewGO<EffectEmitter>(0);
		m_effect->Init(0);
		m_effect->SetScale({ 10.0f,10.0f,10.0f });
		m_effect->SetPosition(m_position + corre2);
		m_effect->SetRotation(m_rotation);
		m_effect->Play();
	}
}

void E_001_enemy::ProcessCommonStateTransition()
{
	//�e�^�C�}�[���������B
	m_idleTimer = 0.0f;
	m_chaseTimer = 0.0f;
	//�G�l�~�[����v���C���[�Ɍ������x�N�g�����v�Z����B
	Vector3 diff = m_player->Getposition() - m_position;
	Vector3 playerPosition = m_player->Getposition();
	//�v���C���[����������B
	if (SearchPlayer() == true)
	{
		//�x�N�g���𐳋K������B
		diff.Normalize();
		//�ړ����x��ݒ肷��B
		m_movespeed = diff * enemyspeed;
		//�U���ł��鋗�����ǂ���
		if (SearchAttackDistance() == true){
			if (m_attackTimer < 0.0f){
				if (SearchAttackDistanceNear() == true){
					m_enemystate = enEnemyState_AttackNear;
				}
				else {
					//�v���C���[�ʒu�擾
					m_targetPosition = playerPosition;
					m_enemystate = enEnemyState_Attack;
				}
				m_attackTimer = attacktime;
				return;
			}
			//���݂̃X�e�[�g���U��
			if (m_enemystate == enEnemyState_Attack||
				m_enemystate == enEnemyState_AttackNear){
				//�A���Ō������Ȃ��悤��
				//�ǐ�
				m_enemystate = enEnemyState_Chase;
				return;
			}
			//���݂̃X�e�[�g���U���łȂ�
			else{
				m_enemystate = enEnemyState_Chase;
				return;
			}
		}
	}
	//�v���C���[���������Ȃ���΁B
	else
	{
		//�ҋ@�X�e�[�g�ɑJ�ڂ���B
		m_attackTimer = attacktime;
		m_enemystate = enEnemyState_Idle;
		return;

	}
}


void E_001_enemy::ProcessIdleStateTransition()
{
	ProcessCommonStateTransition();
}

void E_001_enemy::ProcessChaseStateTransition()
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

void E_001_enemy::ProcessAttackStateTransition()
{
	//�U���A�j���[�V�����̍Đ����I�������B
	if (m_modelrender.IsPlayingAnimation() == false)
	{
		//���̃X�e�[�g�ɑJ�ڂ���B
		ProcessCommonStateTransition();
	}
}

void E_001_enemy::ProcessReceiveDamageStateTransition()
{
	//��_���[�W�A�j���[�V�����̍Đ����I�������B
	if (m_modelrender.IsPlayingAnimation() == false)
	{
		if (SearchAttackDistanceNear() == true){
			m_enemystate = enEnemyState_AttackNear;
		}
		else{
			m_enemystate = enEnemyState_Attack;
		}
	}
}

void E_001_enemy::ProcessDownStateTransition()
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

void E_001_enemy::Render(RenderContext& rc)
{
	m_modelrender.Draw(rc);
	//if (m_enemysheld_type == SheldEnemy) {
	//	if (m_effectSH->IsPlay() == false) {
	//		Shplay();
	//	}
	//}
}