#include "stdafx.h"
#include "Ca_maincamera.h"

#include "P_main_Player.h"

bool Ca_maincamera::Start()
{
	//�����_���王�_�܂ł̃x�N�g����ݒ�B
	m_Campos.Set(0.0f, 100.0f, -200.0f);
	m_player = FindGO<P_main_Player>("player");
	//�΂˃J�����̏������B
	m_springCamera.Init(
		*g_camera3D,		//�΂˃J�����̏������s���J�������w�肷��B
		1000.0f,			//�J�����̈ړ����x�̍ő�l�B
		true,				//�J�����ƒn�`�Ƃ̂����蔻�����邩�ǂ����̃t���O�Btrue���Ƃ����蔻����s���B
		5.0f				//�J�����ɐݒ肳��鋅�̃R���W�����̔��a�B��R������true�̎��ɗL���ɂȂ�B
	);
	return true;
}

void Ca_maincamera::Update()
{
	if (m_player->IsLockOn() == false)
	{
		//�ʒu�X�V
		CamposUpdate();
	}
	else
	{
		ProcessLockOn();
	}
	//�J�����X�V
	m_springCamera.Update();
}

void Ca_maincamera::CamposUpdate()
{

	//�J�������X�V�B
	//�����_���v�Z����B
	Vector3 target = m_player->Getposition();
	//�v���C���̑������炿����Ə�𒍎��_�Ƃ���B
	target.y += 80.0f;
	target += g_camera3D->GetForward() * 20.0f;

	Vector3 toCameraPosOld = m_Campos;
	//�p�b�h�̓��͂��g���ăJ�������񂷁B
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();
	//Y������̉�]
	Quaternion qRot;
	qRot.SetRotationDeg(Vector3::AxisY, 1.5f * x);
	qRot.Apply(m_Campos);
	//X������̉�]�B
	Vector3 axisX;
	axisX.Cross(Vector3::AxisY, m_Campos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, 1.5f * y);
	qRot.Apply(m_Campos);
	//�J�����̉�]�̏�����`�F�b�N����B
	//�����_���王�_�܂ł̃x�N�g���𐳋K������B
	//���K������ƁA�x�N�g���̑傫�����P�ɂȂ�B
	//�傫�����P�ɂȂ�Ƃ������Ƃ́A�x�N�g�����狭�����Ȃ��Ȃ�A�����݂̂̏��ƂȂ�Ƃ������ƁB
	Vector3 toPosDir = m_Campos;
	toPosDir.Normalize();
	if (toPosDir.y < -0.5f) {
		//�J����������������B
		m_Campos = toCameraPosOld;
	}
	else if (toPosDir.y > 0.8f) {
		//�J�����������������B
		m_Campos = toCameraPosOld;
	}


	//���_���v�Z����B
	Vector3 pos = target + m_Campos;

	//�o�l�J�����ɒ����_�Ǝ��_��ݒ肷��B
	m_springCamera.SetPosition(pos);
	m_springCamera.SetTarget(target);
}

void Ca_maincamera::ProcessLockOn()
{
	Vector3 target = m_player->GetTargetPosition();
	Vector3 toPos = target - m_player->Getposition();
	float r = toPos.Length() + 200.0f;
	toPos.y = 0.0f;
	toPos.Normalize();
	Vector3 position = target - toPos * r;
	position.y = m_player->Getposition().y + 200.0f;

	m_springCamera.SetPosition(position);
	m_springCamera.SetTarget(target);
}