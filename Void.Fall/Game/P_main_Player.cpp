#include "stdafx.h"
#include "P_main_Player.h"
///////////////////////////////////////////////////////////
#define playerspeed 250.0f			//�v���C���[�X�s�[�h
#define playerjamp 400.0f			//�v���C���[�W�����v

bool P_main_Player::Start()
{
	//�A�j���[�V�����ǂݍ���
	m_animationclips[enAnimationClip_Idle].Load("Assets/animData/Player/idle.tka");
	m_animationclips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationclips[enAnimationClip_Walk].Load("Assets/animData/Player/walk.tka");
	m_animationclips[enAnimationClip_Walk].SetLoopFlag(true);
	//enAnimationClip_Attack:�A�j���[�V�����L�[name(magic_attack)
	m_animationclips[enAnimationClip_Attack].Load("Assets/animData/Player/attack1.tka");
	m_animationclips[enAnimationClip_Attack].SetLoopFlag(false);


	m_modelrender = new ModelRender;
	m_modelrender->Init("Assets/modelData/A_testPlayer/RE_Player.tkm", m_animationclips, enAnimationClip_Num);
	m_charaCon.Init(25.0f, 70.0f, m_position);

	//�A�j���[�V�����C�x���g�p�֐��ݒ�
	m_modelrender->AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	return true;
}

void P_main_Player::Update()
{
	//�ړ�����
	Move();
	//��]����
	Rotation();
	//�A�j���[�V����
	PlayAnimation();
	//�X�e�[�g�̑J�ڏ���
	ManageState();
	//�`�揈��
	m_modelrender->Update();

}

void P_main_Player::PlayAnimation()
{
	m_modelrender->SetAnimationSpeed(1.0f);
	switch (m_playerstate)
	{
		//�ҋ@
	case enPlayerState_Idle:
		m_modelrender->PlayAnimation(enAnimationClip_Idle, 0.1f);
		break;
		//����
	case enPlayerState_Walk:
		m_modelrender->PlayAnimation(enAnimationClip_Walk, 0.1f);
		break;
	}

}

void P_main_Player::Render(RenderContext& rc)
{
	m_modelrender->Draw(rc);
}

void P_main_Player::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	if (wcscmp(eventName, L"magic_attack") == 0) {
	}
}

void P_main_Player::Move()
{
	//�ړ��ł��Ȃ���Ԃł���΁A�ړ������͂��Ȃ��B
	if (IsEnableMove() == false)
	{
		return;
	}
	m_movespeed.x = 0.0f;
	m_movespeed.z = 0.0f;

	//���̃t���[���̈ړ��ʂ����߂�B
	//���X�e�B�b�N�̓��͗ʂ��󂯎��B
	float lStick_x = g_pad[0]->GetLStickXF();
	float lStick_y = g_pad[0]->GetLStickYF();
	//�J�����̑O�������ƉE�������擾�B
	Vector3 cameraForward = g_camera3D->GetForward();
	Vector3 cameraRight = g_camera3D->GetRight();
	//XZ���ʂł̑O�������A�E�����ɕϊ�����B
	cameraForward.y = 0.0f;
	cameraForward.Normalize();
	cameraRight.y = 0.0f;
	cameraRight.Normalize();
	//XZ�����̈ړ����x���N���A�B
	m_movespeed += cameraForward * lStick_y * playerspeed;	//�������ւ̈ړ����x�����Z�B
	m_movespeed += cameraRight * lStick_x * playerspeed;		//�E�����ւ̈ړ����x�����Z�B
	if (g_pad[0]->IsTrigger(enButtonA) //A�{�^���������ꂽ��
		&& m_charaCon.IsOnGround()   //���A�n�ʂɋ�����
		) {
		//�W�����v����B
		m_movespeed.y = playerjamp;	//������ɑ��x��ݒ肵�āA
	}
	m_movespeed.y -= 980.0f * g_gameTime->GetFrameDeltaTime();
	//�L�����N�^�[�R���g���[���[���g�p���āA���W���X�V�B
	m_position = m_charaCon.Execute(m_movespeed, g_gameTime->GetFrameDeltaTime());
	if (m_charaCon.IsOnGround()) {
		//�n�ʂɂ����B
		m_movespeed.y = 0.0f;
	}
	Vector3 modelPosition = m_position;
	//������Ƃ������f���̍��W��������B
	modelPosition.y += 2.5f;
	m_modelrender->SetPosition(modelPosition);
}

void P_main_Player::Rotation()
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

	//�v���C���[�̐��ʃx�N�g�����v�Z����B
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}

void P_main_Player::ManageState()
{
	switch (m_playerstate)
	{
	case enPlayerState_Idle:
		//�ҋ@�X�e�[�g�J��
		ProcessIdleStateTransition();
		break;
	case enPlayerState_Walk:
		//�����X�e�[�g�J��
		ProcessWalkStateTransition();
		break;
	}
}

void P_main_Player::ProcessIdleStateTransition()
{
	ProcessCommonStateTransition();
}

void P_main_Player::ProcessWalkStateTransition()
{
	ProcessCommonStateTransition();
}

void P_main_Player::ProcessCommonStateTransition()
{
	//x��z�̈ړ����x����������(�X�e�B�b�N�̓��͂���������)�B
	if (fabsf(m_movespeed.x) >= 0.001f || fabsf(m_movespeed.z) >= 0.001f)
	{
		if (m_charaCon.IsOnGround() == true)
		{
			//�����ɂ���B
			m_playerstate = enPlayerState_Walk;
		}
	}
	//x��z�̈ړ����x������������(�X�e�B�b�N�̓��͂�����������)�B
	else
	{
		//�X�e�[�g��ҋ@�ɂ���B
		if (m_charaCon.IsOnGround() == true)
		{
			m_playerstate = enPlayerState_Idle;
		}
		return;
	}
}
