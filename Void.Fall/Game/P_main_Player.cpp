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
	m_animationclips[enAnimationClip_ReceiveDamage].Load("Assets/animData/Player/receivedamage.tka");
	m_animationclips[enAnimationClip_ReceiveDamage].SetLoopFlag(false);
	m_animationclips[enAnimationClip_Down].Load("Assets/animData/Player/down.tka");
	m_animationclips[enAnimationClip_Down].SetLoopFlag(false);


	m_modelrender = new ModelRender;
	m_modelrender->Init("Assets/modelData/A_testPlayer/RE_Player.tkm", m_animationclips, enAnimationClip_Num);
	m_charaCon.Init(25.0f, 70.0f, m_position);

	m_spriterender.Init("Assets/sprite/testlock/lock.dds", 200, 200);
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
	//�����蔻�菈��
	Collision();
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
	m_spriterender.Update();

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
	case enPlayerState_Avoidance:
		//����A�j���[�V����
		m_modelrender->PlayAnimation(enAnimationClip_Avoidance, 0.1f);
		break;
	case enPlayerState_ReceiveDamage:
		//��_���A�j���[�V����
		m_modelrender->PlayAnimation(enAnimationClip_ReceiveDamage, 0.1f);
		break;
	case enPlayerState_Down:
		//�_�E���A�j���[�V����
		m_modelrender->PlayAnimation(enAnimationClip_Down, 0.1f);
		break;
	}
}

void P_main_Player::Render(RenderContext& rc)
{
	m_modelrender->Draw(rc);
	if (m_isTakeAim == false)
	{
		return;
	}
	m_spriterender.Draw(rc);
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
	if (IsEnableMove() == true){
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
	if (m_isLockOn == true){
		if (m_playerstate == enPlayerState_Attack){
			AttackRotation();
			return;
		}
	}
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

void P_main_Player::AttackRotation()
{
}

void P_main_Player::Collision()
{
	//���G���Ԓ��͏������Ȃ�
	if (m_mutekitimer > 0)
	{
		m_mutekitimer -= g_gameTime->GetFrameDeltaTime();
		return;
	}
	//��_���[�W�A��𒆁A���邢�̓_�E���X�e�[�g�̎��́B
//�����蔻�菈���͂��Ȃ��B
	if (m_playerstate == enPlayerState_ReceiveDamage ||
		m_playerstate == enPlayerState_Avoidance || 
		m_playerstate == enPlayerState_Down)
	{
		return;
	}
	//�G�l�~�[�̍U���p�̃R���W�������擾����B
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("enemy_attack");
	//�R���W�����̔z���for���ŉ񂷁B
	for (auto collision : collisions)
	{
		//�R���W�����ƃL�����R�����Փ˂�����B
		if (collision->IsHit(m_charaCon)){
			//HP��1���炷�B
			m_hp -= 1;
			//HP��0�ɂȂ�����B
			if (m_hp == 0) {
				//�_�E���X�e�[�g�ɑJ�ڂ���B
				m_playerstate = enPlayerState_Down;
			}
			else {
				//��_���[�W�X�e�[�g�ɑJ�ڂ���B
				m_playerstate = enPlayerState_ReceiveDamage;
			}
		}
	}
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
	case enPlayerState_ReceiveDamage:
		//��_���J��
		ProcessReceiveDamageStateTransition();
		break;
	case enPlayerState_Down:
		//�_�E���J��
		ProcessDownStateTransition();
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
		//�X�L����ʏ��ԂɕύX����
		NormalTex();
		//�X�e�[�g��ҋ@�X�e�[�g�ɂ���
		m_playerstate = enPlayerState_Idle;
		//����N�[���_�E���^�C�}�[������������B
		m_Avoidbreaktimer = Avoidbreaktime;
	}
}

void P_main_Player::ProcessReceiveDamageStateTransition()
{
	//��_���[�W�A�j���[�V�����̍Đ����I�������B
	if (m_modelrender->IsPlayingAnimation() == false)
	{
		//���G�^�C�}�[��������
		m_mutekitimer = mutekitime;
		//�X�e�[�g��J�ڂ���B
		ProcessCommonStateTransition();
	}
}

void P_main_Player::ProcessDownStateTransition()
{
	//�_�E���A�j���[�V�����̍Đ����I�������B
	if (m_modelrender->IsPlayingAnimation() == false)
	{
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
		//�X�e�B�b�N����̓��͂�����
		if (fabsf(m_movespeed.x) >= 0.001f || fabsf(m_movespeed.z) >= 0.001f) {
			//Y�{�^���������ꂽ��
			if (g_pad[0]->IsTrigger(enButtonY)) {
				//�X�L���������ԂɕύX��
				AvoidanceTex();
				//����X�e�[�g�ɂ���
				m_playerstate = enPlayerState_Avoidance;
				//����^�C�}�[�������l�ɂ���
				m_Avoidancetimer = Avoidancetime;
				return;
			}
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

void P_main_Player::NormalTex()
{
	delete m_modelrender;
	//�A�j���[�V�����ǂݍ���
	m_animationclips[enAnimationClip_Idle].Load("Assets/animData/Player/idle.tka");
	m_animationclips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationclips[enAnimationClip_Walk].Load("Assets/animData/Player/walk.tka");
	m_animationclips[enAnimationClip_Walk].SetLoopFlag(true);
	//enAnimationClip_Attack:�A�j���[�V�����L�[name(magic_attack)
	m_animationclips[enAnimationClip_Attack].Load("Assets/animData/Player/attack1.tka");
	m_animationclips[enAnimationClip_Attack].SetLoopFlag(false);
	m_animationclips[enAnimationClip_ReceiveDamage].Load("Assets/animData/Player/receivedamage.tka");
	m_animationclips[enAnimationClip_ReceiveDamage].SetLoopFlag(false);
	m_modelrender = new ModelRender;
	m_modelrender->Init("Assets/modelData/A_testPlayer/RE_Player.tkm",
		m_animationclips, enAnimationClip_Num);
	m_modelrender->SetRotation(m_rotation);
	m_modelrender->SetPosition(m_position);
	//�A�j���[�V�����C�x���g�p�֐��ݒ�
	m_modelrender->AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

}

void P_main_Player::AvoidanceTex()
{
	delete m_modelrender;
	m_animationclips_sub[enAnimationClip_Avoidance].Load("Assets/animData/Player/avoidance.tka");
	m_animationclips_sub[enAnimationClip_Avoidance].SetLoopFlag(true);
	m_modelrender = new ModelRender;
	m_modelrender->Init("Assets/modelData/A_testPlayer/RE_Player_Avoid.tkm",
		m_animationclips_sub, enAnimationClip_sub_Num);
	m_modelrender->SetRotation(m_rotation);
	m_modelrender->SetPosition(m_position);
}

void P_main_Player::Takeaim()
{
	//�G�l�~�[�̐���0�������珈�����Ȃ��B
	if (m_numenemy == 0){
		return;
	}
	//�G�l�~�[�����v�Z
	if (m_isLockOn == false){
		for (int i = 0; i < m_numenemy - 1; i++) {
			Vector3 enemypos1 = *m_enemyPositionList[i];
			Vector3 enemypos2 = *m_enemyPositionList[i + 1];
			Vector3 diff1 = enemypos1 - m_position;
			Vector3 diff2 = enemypos2 - m_position;
			Vector3 diff3 = enemypossub - m_position;
			if (diff1.Length() < diff2.Length()) {
				if (diff1.Length() < diff3.Length()) {
					enemypossub = enemypos1;
				}
			}
			else {
				if (diff2.Length() < diff3.Length()) {
					enemypossub = enemypos2;
				}
			}
		}
	}

	Vector3 diff4 = enemypossub - m_position;
	if (diff4.Length() >= 700.0f)
	{
		m_isTakeAim = false;
		return;
	}

	enemypossub.y = 60.0f;
	//enemypossub.Normalize();

	float angle = acosf(m_forward.Dot(enemypossub));

	//�v���C���[�̐��ʃx�N�g���ƁB
	//�v���C���[����G�l�~�[�Ɍ������x�N�g���́B
	//�p�x��90�x�ȏォ�B
	//���b�N�I����ԂłȂ�������B
	//�^�[�Q�b�e�B���O���Ȃ��B
	if (angle > Math::PI / 2 && m_isLockOn == false)
	{
		m_isTakeAim = false;
		return;
	}
	m_isTakeAim = true;

	Vector2 screenPosition;
	g_camera3D->CalcScreenPositionFromWorldPosition(screenPosition, enemypossub);
	m_spriterender.SetPosition(Vector3(screenPosition.x, screenPosition.y, 0.0f));
	m_targetPosition = enemypossub;
}

void P_main_Player::Lockon()
{
	//�^�[�Q�b�e�B���O������Ă��Ȃ�������B
//���b�N�I�����Ȃ��B
	if (m_isTakeAim == false)
	{
		m_isLockOn = false;
		return;
	}

	if (g_pad[0]->IsTrigger(enButtonX))
	{
		//���b�N�I�����Ă��Ȃ���΁B
		if (m_isLockOn == false)
		{
			//���b�N�I������B
			m_isLockOn = true;
		}
		//���b�N�I�����Ă���΁B
		else
		{
			//���b�N�I�����Ȃ��B
			m_isLockOn = false;
		}
	}
}
