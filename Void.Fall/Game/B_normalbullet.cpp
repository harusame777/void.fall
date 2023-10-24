#include "stdafx.h"
#include "B_normalbullet.h"
#include "collision/CollisionObject.h"
#include "P_main_Player.h"
#include "IEnemy.h"
namespace
{
	const Vector3 scale = { 1.5f,1.5f,1.5f }; //�傫��
	const Vector3 corre = { 0.0f,40.0f,0.0f };//�ʒu�C��
}

bool B_normalbullet::Start()
{

	//�A�j���[�V�����ǂݍ���
	m_animationclips[enAnimationClip_Move].Load
	("Assets/modelData/A_attack/bullet/normalbullet/normalbullet.tka");
	m_animationclips[enAnimationClip_Move].SetLoopFlag(true);

	//���f���ǂݍ���
	m_modelrender = new ModelRender;
	m_modelrender->Init("Assets/modelData/A_attack/bullet/normalbullet/normalbullet1.tkm",
		m_animationclips, enAnimationClip_Num);

	m_modelrender->SetPosition(m_position);
	m_modelrender->SetScale(scale);

	

	//�ړ����x���v�Z�B
	m_velocity = Vector3::AxisZ;
	m_rotation.Apply(m_velocity);
	m_position += m_velocity * 50.0f;
	m_velocity *= 1200.0f;
	m_rotation.AddRotationDegY(360.0f);

	//�R���W�����I�u�W�F�N�g���쐬����B
	m_collisionObject = NewGO<CollisionObject>(0);
	//����̃R���W�������쐬����B
	m_collisionObject->CreateSphere(m_position, Quaternion::Identity, 20.0f * m_scale.z);
	m_collisionObject->SetName("player_attack");
	//�R���W�����I�u�W�F�N�g�������ō폜����Ȃ��悤�ɂ���B
	m_collisionObject->SetIsEnableAutoDelete(false);

	m_ienemy = FindGO<IEnemy>("ienemy");

	return true;
}

void B_normalbullet::Update()
{
	//�e�ۈړ�
	Movebullet();
	//��]����
	Rotation();
	//�e�ێ��ԏ��ŏ���
	Inpacttime();
	//�e�ۑΏۏՓˏ���
	Inpacthit();
	//�A�j���[�V����
	PlayAnimation();
	//�`�揈��
	m_modelrender->Update();
}

void B_normalbullet::Movebullet()
{
	//���W���ړ�������B
	m_position += m_velocity *g_gameTime->GetFrameDeltaTime();
	m_modelrender->SetPosition(m_position);
	m_collisionObject->SetPosition(m_position);
	bullettime -= g_gameTime->GetFrameDeltaTime();
}

void B_normalbullet::Rotation()
{
	//���m�ɂ͒e�ۂ��ړ������Ɍ�������v���O����
		//�ړ����x�����ɓ����Ƃł���B
	float angle = atan2(-m_velocity.x, m_velocity.z);
	m_rotation.SetRotationY(-angle);
	m_modelrender->SetRotation(m_rotation);
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}

void B_normalbullet::Inpacttime()
{
	if (bullettime > 0)
	{
		return;
	}
	DeleteGO(m_collisionObject);
	delete m_modelrender;
	DeleteGO(this);
}

void B_normalbullet::Inpacthit()
{
	//�v���C���[�̍U���p�̃R���W�������擾����B
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("enemy_col");
	//�R���W�����̔z���for���ŉ񂷁B
	for (auto collision : collisions)
	{
		//�R���W�����ƃL�����R�����Փ˂�����B
		if (collision->IsHit(m_collisionObject))
		{
			Delay();
			if (bullettdelaytime > 0)
			{
				DeleteGO(m_collisionObject);
				delete m_modelrender;
				DeleteGO(this);
			}
		}
	}
}

void B_normalbullet::Delay()
{
	bullettdelaytime -= g_gameTime->GetFrameDeltaTime();
	return;
}

void B_normalbullet::PlayAnimation()
{
	m_modelrender->PlayAnimation(enAnimationClip_Move, 0.1f);
}

void B_normalbullet::Render(RenderContext& rc)
{
	m_modelrender->Draw(rc);
}