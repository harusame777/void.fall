#include "stdafx.h"
#include "E_002_enemy.h"
#include "IEnemy.h"
#include "Game.h"
bool E_002_enemy::Start()
{

	//�A�j���[�V�����ǂݍ���
	m_animationclips[enAnimationClip_Idle].Load("Assets/animData/Enemy/enemy_002/idle.tka");
	m_animationclips[enAnimationClip_Idle].SetLoopFlag(true);

	//���f���ǂݍ���
	m_modelrender = new ModelRender;
	m_modelrender->Init("Assets/modelData/Enemy/enemy_002/enemy_002.tkm", m_animationclips, enAnimationClip_Num);

	//��]
	m_modelrender->SetRotation(m_rotation);
	//���W
	m_modelrender->SetPosition(m_position);
	//�X�P�[��
	m_modelrender->SetScale(m_scale);
	//�L�����R��������
	m_charaCon.Init(20.0f, 100.0f, m_position);

	return true;
}

void E_002_enemy::Render(RenderContext& rc)
{
	m_modelrender->Draw(rc);
}

void E_002_enemy::Update()
{
	m_modelrender->Update();
}

