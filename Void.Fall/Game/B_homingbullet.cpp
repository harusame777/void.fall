#include "stdafx.h"
#include "B_homingbullet.h"
#include "P_main_Player.h"
#include "collision/CollisionObject.h"
//define
namespace
{
	const Vector3 scale = { 1.5f,1.5f,1.5f }; //�傫��
	const Vector3 correE = { 0.0f,60.0f,0.0f };//�ʒu�C��
	const Vector3 correP = { 0.0f,-20.0f,0.0f }; 
	const float bullet_homingtime = 3.0f;     //�ő�ǐՎ���
	const float bullet_sppedE = 10.0f;         //���x
	const float curvatureE = 10.0f;            //�Ȃ��鋭��
	const float bullet_sppedP = 10.0f;         //���x
	const float curvatureP = 20.0f;            //�Ȃ��鋭��
	const float damping = 0.1f;               //����
}

bool B_homingbullet::Start()
{
	if (m_shooter == enShooter_Enemy) {
		//�R���W�����I�u�W�F�N�g���쐬����B
		m_collisionObject = NewGO<CollisionObject>(0);
		//����̃R���W�������쐬����B
		m_collisionObject->CreateSphere(m_position, Quaternion::Identity, 20.0f * m_scale.z);
		m_collisionObject->SetName("enemy_attack");
		//�R���W�����I�u�W�F�N�g�������ō폜����Ȃ��悤�ɂ���B
		m_collisionObject->SetIsEnableAutoDelete(false);

		//�A�j���[�V�����ǂݍ���
		m_animationclips[enAnimationClip_Move].Load
		("Assets/modelData/A_attack/bullet/homingbullet/homingbullet.tka");
		m_animationclips[enAnimationClip_Move].SetLoopFlag(true);

		//���f���ǂݍ���
		m_modelrender = new ModelRender;
		m_modelrender->Init("Assets/modelData/A_attack/bullet/homingbullet/homingbullet1.tkm",
			m_animationclips, enAnimationClip_Num);
	}
	if (m_shooter == enShooter_Player){
		//�R���W�����I�u�W�F�N�g���쐬����B
		m_collisionObject = NewGO<CollisionObject>(0);
		//����̃R���W�������쐬����B
		m_collisionObject->CreateSphere(m_position, Quaternion::Identity, 20.0f * m_scale.z);
		m_collisionObject->SetName("player_attack");
		//�R���W�����I�u�W�F�N�g�������ō폜����Ȃ��悤�ɂ���B
		m_collisionObject->SetIsEnableAutoDelete(false);

		//�A�j���[�V�����ǂݍ���
		m_animationclips[enAnimationClip_Move].Load
		("Assets/modelData/A_attack/bullet/normalbullet/normalbullet.tka");
		m_animationclips[enAnimationClip_Move].SetLoopFlag(true);

		//���f���ǂݍ���
		m_modelrender = new ModelRender;
		m_modelrender->Init("Assets/modelData/A_attack/bullet/normalbullet/normalbullet1.tkm",
			m_animationclips, enAnimationClip_Num);
	}

	m_modelrender->SetPosition(m_position);
	m_modelrender->SetRotation(m_rotation);
	m_modelrender->SetScale(scale);

	m_player = FindGO<P_main_Player>("player");

	return true;
}

void B_homingbullet::Update()
{
	//�e�ۈړ�
	Movebullet();
	//��]����
	Rotation();
	//�e�ێ��ԏ��ŏ���
	Inpacttime();
	//�e�ۑΏۏՓˏ���
	Inpacthit();
	//�e�ۏ�������
	deletebullet();
	//�A�j���[�V����
	PlayAnimation();
	//�`�揈��
	m_modelrender->Update();
}

void B_homingbullet::PlayAnimation()
{
	m_modelrender->PlayAnimation(enAnimationClip_Move, 0.1f);
}

void B_homingbullet::Movebullet()
{
	if (bullettime > bullet_homingtime){
		switch (m_shooter)
		{
		case B_homingbullet::enShooter_Player:
			CalcVelocity(bullet_sppedP, curvatureP, damping);
			break;
		case B_homingbullet::enShooter_Enemy:
			CalcVelocity(bullet_sppedE, curvatureE, damping);
			break;
		}
	}
	m_position += m_velocity;
	bullettime -= g_gameTime->GetFrameDeltaTime();
	switch (m_shooter)
	{
	case enShooter_Player:
		SetTarget(m_player->m_targetPosition);
		break;
	case enShooter_Enemy:
		SetTarget(m_player->m_position);
		break;
	}
	m_collisionObject->SetPosition(m_position);
	m_modelrender->SetPosition(m_position);
}

void B_homingbullet::Rotation()
{
	//���m�ɂ͒e�ۂ��ړ������Ɍ�������v���O����
			//�ړ����x�����ɓ����Ƃł���B
	float angle = atan2(-m_velocity.x, m_velocity.z);
	m_rotation.SetRotationY(-angle);
	m_modelrender->SetRotation(m_rotation);
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}

void B_homingbullet::CalcVelocity(const float speed, const float curvatureRadius,
	const float damping)
{
	float maxCentripetalAccel = speed * speed / curvatureRadius;
	float propulsion = speed * damping;

	switch (m_shooter)
	{
	case enShooter_Player:
		targetPosition = m_targetPosition += correP;
		break;
	case enShooter_Enemy:
		targetPosition = m_targetPosition += correE;
		break;
	}
	Vector3 toTarget = targetPosition - m_position;
	Vector3 vn = m_velocity;
	vn.Normalize();
	float dot = toTarget.Dot(vn);
	Vector3 centripetalAccel = toTarget - (vn * dot);
	float centripetalAccelMagnitude = centripetalAccel.Length();
	if (centripetalAccelMagnitude > 1.0f)
	{
		centripetalAccel /= centripetalAccelMagnitude;
	}
	Vector3 force = centripetalAccel * curvatureRadius;
	force += vn * propulsion;
	force -= m_velocity * damping;
	m_velocity += force * g_gameTime->GetFrameDeltaTime();
}

void B_homingbullet::Inpacttime()
{
	if (bullettime > 0)
	{
		return;
	}
	DeleteGO(m_collisionObject);
	delete m_modelrender;
	DeleteGO(this);
}

void B_homingbullet::Inpacthit()
{
	if (m_shooter == enShooter_Player){
		//enemy�̃R���W�������擾����B							//��enemy�̋��ʃR���W����
		const auto& collisions = g_collisionObjectManager->FindCollisionObjects("enemy_col");
		//�R���W�����̔z���for���ŉ񂷁B
		for (auto collision : collisions) {
			//�R���W�����ƃL�����R�����Փ˂�����B
			if (collision->IsHit(m_collisionObject)) {
				if (m_isDelete == false) {
					m_isDelete = true;	//deletebullet��if�����ʂ�悤�ɂ���B
					m_deleteTimer = deletetimer; //deletetimer�͌���0.2f�B
				}
			}
		}
	}
	if (m_shooter == enShooter_Enemy)
	{
		if (m_collisionObject->IsHit(m_player->m_charaCon)) {
			if (m_isDelete == false) {
				m_isDelete = true;	//deletebullet��if�����ʂ�悤�ɂ���B
				m_deleteTimer = deletetimer; //deletetimer�͌���0.2f�B
			}
		}
	}
}

void B_homingbullet::deletebullet()
{      //��Inpacthit��true�ɂ���B
	if (m_isDelete) {
		m_deleteTimer -= g_gameTime->GetFrameDeltaTime(); //deletetimer��1�t���[������
		//���炷�B
			//���^�C�}�[���[���ɂȂ�����B(deletetimer���0�̕����傫���Ȃ�����)
		if (m_deleteTimer <= 0.0f) {
			DeleteGO(m_collisionObject);//��������
			delete m_modelrender;
			DeleteGO(this);
		}
	}
}

void B_homingbullet::Render(RenderContext& rc)
{
	m_modelrender->Draw(rc);
}