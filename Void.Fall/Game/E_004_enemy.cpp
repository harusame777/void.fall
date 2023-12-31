#include "stdafx.h"
#include "E_004_enemy.h"
#include "M_parts4.h"
#include "math.h"

namespace
{
	const Vector3 corre1 = { 0.0f,60.0f,0.0f };//�ʒu�C��
}

bool E_004_enemy::Start()
{
	//m_animationclips[enAnimationClip_Standby].Load("Assets/animData/Enemy/enemy_004/standby.tka");
	//m_animationclips[enAnimationClip_Standby].SetLoopFlag(true);


	m_modelrender.Init("Assets/modelData/Enemy/enemy_004/enemy_004.tkm"
	/*, m_animationclips, enAnimationClip_Num*/);
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
	m_parts4 = FindGO<M_parts4>("parts4");

	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);

	EnemyGoEffect();

	return true;
}

void E_004_enemy::Update()
{
	//�����X�e�[�g���N����Ԃ�������B
	if (m_enemystate == enEnemyState_Active){
		//�N����Ԏ��̏��������s����B
		ActiveLock();
	}
	//�����蔻�菈��
	Collision();
	//�A�j���[�V����
	PlayAnimation();
	//�`�揈��
	m_modelrender.Update();
}

void E_004_enemy::PlayAnimation()
{
	//m_modelrender.SetAnimationSpeed(1.0f);
	//switch (m_enemystate)
	//{
	//case E_004_enemy::enEnemyState_Standby:
	//	m_modelrender.PlayAnimation(enAnimationClip_Standby, 0.1f);
	//	break;
	//}
}

void E_004_enemy::Collision()
{
	//�v���C���[�̍U���p�̃R���W�������擾����B
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_attack");
	//�R���W�����̔z���for���ŉ񂷁B
	for (auto collision : collisions)
	{
		//�R���W�����ƃL�����R�����Փ˂�����B
		if (collision->IsHit(m_collisionObject))
		{
			if (m_enemystate == enEnemyState_Standby){
				m_enemystate = enEnemyState_Active;
				m_Locktimer = Locktime;
				m_parts4->mapLockOn();
				EnemySummon();
			}
		}
	}
}

void E_004_enemy::ActiveLock()
{
	m_Locktimer -= g_gameTime->GetFrameDeltaTime();
	float timer0 = fmod(m_Locktimer,20.0f);
	if (timer0 == 0){
		EnemySummon();
	}
	if (m_Locktimer > 0){

	}
}

void E_004_enemy::EnemySummon()
{

}

void E_004_enemy::Render(RenderContext& rc)
{
	m_modelrender.Draw(rc);
}


