#include "stdafx.h"
#include "Ob_savepoint.h"
#include "P_main_Player.h"
#include "Game.h"

namespace
{
	const Vector3 corre1 = { 0.0f,60.0f,0.0f };//�ʒu�C��
}

bool Ob_savepoint::Start()
{
	m_animationclips[enAnimationClip_Standby].Load("Assets/modelData/A_object/savepoint/normal.tka");
	m_animationclips[enAnimationClip_Standby].SetLoopFlag(true);
	m_animationclips[enAnimationClip_Save].Load("Assets/modelData/A_object/savepoint/save.tka");
	m_animationclips[enAnimationClip_Save].SetLoopFlag(false);

	//���f���ǂݍ���
	m_modelrender.Init("Assets/modelData/A_object/savepoint/savepoint.tkm"
		, m_animationclips, enAnimationClip_Num);
	//���W�ݒ�
	m_modelrender.SetPosition(m_position);
	//��]�l�ݒ�
	m_modelrender.SetRotation(m_rotation);
	//�傫���ݒ�
	m_modelrender.SetScale(m_scale);
	m_modelrender.Update();

	//�����蔻��쐬
	m_physicsStaticObject.CreateFromModel(m_modelrender.GetModel(), 
	m_modelrender.GetModel().GetWorldMatrix());
	//�R���W�����I�u�W�F�N�g���쐬����B
	m_collisionObject = NewGO<CollisionObject>(0);
	//����̃R���W�������쐬����B
	m_collisionObject->CreateSphere(m_position, Quaternion::Identity, 60.0f * m_scale.z);
	m_collisionObject->SetName("enemy_col");
	m_collisionObject->SetPosition(m_position + corre1);
	//�R���W�����I�u�W�F�N�g�������ō폜����Ȃ��悤�ɂ���B
	m_collisionObject->SetIsEnableAutoDelete(false);

	m_game = FindGO<Game>("game");
	m_player = FindGO<P_main_Player>("player");

	SetsaveVec();
	return true;
}

void Ob_savepoint::Update()
{
	//�����蔻�菈��
	Collision();
	//�A�j���[�V��������
	PlayAnimation();
	//�J�ڏ���
	ManageState();
	//�`�揈��
	m_modelrender.Update();
}

void Ob_savepoint::Collision()
{
	//�v���C���[�̍U���p�̃R���W�������擾����B
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_attack");
	//�R���W�����̔z���for���ŉ񂷁B
	for (auto collision : collisions)
	{
		//�R���W�����ƃL�����R�����Փ˂�����B
		if (collision->IsHit(m_collisionObject))
		{
			m_state = en_save;
		}
	}
}

void Ob_savepoint::PlayAnimation()
{
	switch (m_state)
	{
	case Ob_savepoint::en_Standby:
		m_modelrender.PlayAnimation(enAnimationClip_Standby, 0.1f);
		break;
	case Ob_savepoint::en_save:
		m_modelrender.PlayAnimation(enAnimationClip_Save, 0.1f);
		break;
	}
}

void Ob_savepoint::ManageState()
{
	switch (m_state)
	{
	case Ob_savepoint::en_Standby:
		return;
		break;
	case Ob_savepoint::en_save:
		ProcessSaveStateTransition();
		break;
	}
}

void Ob_savepoint::ProcessSaveStateTransition()//�Z�[�u�J��
{
	//�A�j���[�V�����̍Đ����I�������B
	if (m_modelrender.IsPlayingAnimation() == false)
	{
		m_game->m_Nowsavepointnum = savenum;
		m_state = en_Standby;
	}
}

void Ob_savepoint::Render(RenderContext& rc){
	//�`�揈��
	m_modelrender.Draw(rc);
}

