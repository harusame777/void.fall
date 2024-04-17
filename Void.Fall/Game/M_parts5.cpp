#include "stdafx.h"
#include "M_parts5.h"
#include "Game.h"
#include "P_main_Player.h"
#include "GameClear.h"

namespace
{
	const Vector3 corre1 = { 0.0f,60.0f,0.0f };//�ʒu�C��
}

bool M_parts5::Start()
{
	//���f���ǂݍ���
	m_modelrender.Init("Assets/modelData/A_mapParts/stage1/mapparts5.tkm");
	//���W�ݒ�
	m_modelrender.SetPosition(m_position);
	//��]�l�ݒ�
	m_modelrender.SetRotation(m_rotation);
	//�傫���ݒ�
	m_modelrender.SetScale(m_scale);
	m_modelrender.Update();

	//�R���W�����I�u�W�F�N�g���쐬����B
	m_collisionObject = NewGO<CollisionObject>(0);
	//����̃R���W�������쐬����B
	m_collisionObject->CreateSphere(m_position, Quaternion::Identity, 60.0f * m_scale.z);
	m_collisionObject->SetName("goal_col");
	m_collisionObject->SetPosition(m_position + corre1);
	//�R���W�����I�u�W�F�N�g�������ō폜����Ȃ��悤�ɂ���B
	m_collisionObject->SetIsEnableAutoDelete(false);

	m_player = FindGO<P_main_Player>("player");
	m_game = FindGO<Game>("game");

	SetPhysics();
	return true;
}

void M_parts5::Update()
{
	//�S�[������o��
	if (GoalLockNum < 0){
		Collision();
	}
	//�`��X�V
	m_modelrender.Update();
}

void M_parts5::Lock()
{
}

void M_parts5::Collision()
{
	if (Deletemap == true){
		if (m_collisionObject->IsHit(m_player->m_charaCon)){
			Deletemap = false;
			NewGO <GameClear>(0, "gameclear");
		}
	}
}

void M_parts5::Render(RenderContext& rc) 
{
	//�`�揈��
	m_modelrender.Draw(rc);
}