#include "stdafx.h"
#include "P_main_Player.h"
#include "B_normalbullet.h"
#include "collision/CollisionObject.h"
#include "IEnemy.h"
#include "Game.h"
///////////////////////////////////////////////////////////
#define playerspeed 250.0f			//�v���C���[�X�s�[�h
#define playerspeedAvoid 250.0f		//����X�s�[�h
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
	//Lockon�ʒu�擾
	Takeaim();
	//���b�N�I��
	Lockon();
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
	case enPlayerState_Idle:
		//�ҋ@�A�j���[�V����
		m_modelrender->PlayAnimation(enAnimationClip_Idle, 0.1f);
		break;
	case enPlayerState_Walk:
		//�����A�j���[�V����
		m_modelrender->PlayAnimation(enAnimationClip_Walk, 0.1f);
		break;
	case enPlayerState_Attack:
		//�U���A�j���[�V����
		m_modelrender->PlayAnimation(enAnimationClip_Attack, 0.3f);
		break;
	}
}

void P_main_Player::Render(RenderContext& rc)
{
	m_modelrender->Draw(rc);
}

void P_main_Player::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//�A�j���[�V�����N���b�v��magic_attack��������
	if (wcscmp(eventName, L"magic_attack") == 0) {
		//normalbullet���쐬����B
		auto bullet = NewGO<B_normalbullet>(0);
		//bullet�̏����ݒ�Ȃ�
		bullet->SetPosition(m_position);
		bullet->Setrotation(m_rotation);
		bullet->SetVelocity(m_forward);
		bullet->m_position.y += 80.0f;
		bullet->m_position.z += 10.0f;
	}
}

void P_main_Player::Move()
{
	//�ړ��ł��Ȃ���Ԃł���΁A�ړ������͂��Ȃ��B
	if (IsEnableMove() == false){
		return;
	}
	//���݂̃X�e�[�g������X�e�[�g��������
	if (m_playerstate == enPlayerState_Avoidance){
		//�������������
		Avoidance();	
		//����^�C�}�[���t���[���P�ʂŌ��炷�B
		m_Avoidancetimer -= g_gameTime->GetFrameDeltaTime();
		return;
	}
	//������Ă��Ȃ�������
	else{
		//����N�[���_�E���^�C�}�[���t���[���P�ʂŌ��炷�B
		m_Avoidbreaktimer -= g_gameTime->GetFrameDeltaTime();
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
	////������Ƃ������f���̍��W��������B
	modelPosition.y += 2.5f;
	m_modelrender->SetPosition(modelPosition);
}

void P_main_Player::Avoidance()
{
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
	m_movespeed += cameraForward * lStick_y * playerspeedAvoid;	//�������ւ̈ړ����x�����Z�B
	m_movespeed += cameraRight * lStick_x * playerspeedAvoid;		//�E�����ւ̈ړ����x�����Z�B
	//�L�����N�^�[�R���g���[���[���g�p���āA���W���X�V�B
	m_position = m_charaCon.Execute(m_movespeed, g_gameTime->GetFrameDeltaTime());
	Vector3 modelPosition = m_position;
	////������Ƃ������f���̍��W��������B
	modelPosition.y += 2.5f;
	m_modelrender->SetPosition(modelPosition);
}

void P_main_Player::Rotation()
{
	//������Ă�����A��]�͂����Ȃ��B
	if (m_playerstate == enPlayerState_Avoidance){
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
	case enPlayerState_Attack:
		//�U���X�e�[�g�J��
		ProcessAttackStateTransition();
		break;
	case enPlayerState_Avoidance:
		//����X�e�[�g�J��
		ProcessAvoidanceStateTransition();
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

void P_main_Player::ProcessAttackStateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelrender->IsPlayingAnimation() == false){
		//�ق��̃X�e�[�g�ɑJ�ڂ���B
		ProcessCommonStateTransition();
	}
}

void P_main_Player::ProcessAvoidanceStateTransition()
{
	//����^�C�}�[��0�ȏゾ������
	if (m_Avoidancetimer >= 0)
	{
		//�ق��̃X�e�[�g�ɑJ�ڂ���B
		ProcessCommonStateTransition();
	}
	//����^�C�}�[��0�ȉ���������
	else
	{
		//�X�e�[�g��ҋ@�X�e�[�g�ɂ���
		m_playerstate = enPlayerState_Idle;
		//����N�[���_�E���^�C�}�[������������B
		m_Avoidbreaktimer = Avoidbreaktime;
	}
}

void P_main_Player::ProcessCommonStateTransition()
{
	//���݂̃X�e�[�g�������������
	if (m_playerstate == enPlayerState_Avoidance)
	{
		//(�ق��̏����������Ȃ�����)return;
		return;
	}
	//B�{�^���������ꂽ��
	if (g_pad[0]->IsTrigger(enButtonB)){
		//attack�X�e�[�g�ɂ���B
		m_playerstate = enPlayerState_Attack;
		return;
	}
	//����N�[���_�E����0�ȉ���
	if (m_Avoidbreaktimer <= 0){
		//Y�{�^���������ꂽ��
		if (g_pad[0]->IsTrigger(enButtonY)) {
			//����X�e�[�g�ɂ���
			m_playerstate = enPlayerState_Avoidance;
			//����^�C�}�[�������l�ɂ���
			m_Avoidancetimer = Avoidancetime;
			return;
		}
	}
	//x��z�̈ړ����x����������(�X�e�B�b�N�̓��͂���������)�B
	if (fabsf(m_movespeed.x) >= 0.001f || fabsf(m_movespeed.z) >= 0.001f){
		//�����X�e�[�g�ɂ���
		m_playerstate = enPlayerState_Walk;
		return;
	}
	//x��z�̈ړ����x������������(�X�e�B�b�N�̓��͂�����������)�B
	else{
		//�ҋ@�X�e�[�g�ɂ���B
		m_playerstate = enPlayerState_Idle;
		return;
	}
}

void P_main_Player::Takeaim()
{
	m_game->enemyList[]
}

void P_main_Player::Lockon()
{

}
