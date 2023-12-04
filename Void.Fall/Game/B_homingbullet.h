#pragma once
class P_main_Player;
class B_homingbullet : public IGameObject
{
public:
///////////////////////////////////////////////////////////
	enum EnShooter {
		enShooter_None,
		enShooter_Player,			//�v���C���[�B
		enShooter_Enemy			//�G�l�~�[�B
	};
	B_homingbullet() {};
	~B_homingbullet() {};
	bool Start();
	void Update();
	void Rotation();
	void Render(RenderContext& rc);
	void Movebullet();
	void Inpacttime();
	void Inpacthit();
	void deletebullet();				//�e�ۏ���
	void CalcVelocity(const float speed, const float curvatureRadius,
		const float damping);
///////////////////////////////////////////////////////////
//�A�j���[�V������/////////////////////////////////////////
	//�A�j���[�V�����X�e�[�g
	enum EnAnimationClip {
		enAnimationClip_Move,//bullet�ړ�
		enAnimationClip_Num//�A�j���[�V������
	};
//�A�j���[�V�����֐�///////////////////////////////////////
	void PlayAnimation();
///////////////////////////////////////////////////////////
//�����ݒ�n��
	void SetVelocity(const Vector3& velocity)
	{
		m_velocity = velocity;
	}
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	void SetTarget(const Vector3& targetPosition)
	{
		m_targetPosition = targetPosition;
	}
	void SetEnShooter(const EnShooter enShooter)
	{
		m_shooter = enShooter;
	}
//�����o�֐��錾
	float bullettime = 6.0f;
	float m_deleteTimer = 0.0f;								//�����f�B���C
	bool m_isDelete = false;								//�����f�B���C�Ɏg�����
	const float deletetimer = 0.1f;							//�f�B���C�^�C�}�[
	Quaternion m_rotation;									//��]
	CollisionObject* m_collisionObject;						//�R���W�����I�u�W�F�N�g�B
	Vector3	m_forward = Vector3::AxisZ;						//�G�l�~�[�̐��ʃx�N�g���B
	Vector3	m_scale = Vector3::One;							//�傫���B
	Vector3 targetPosition;
	P_main_Player* m_player = nullptr;                      //�v���C���[
	AnimationClip m_animationclips[enAnimationClip_Num];    //�A�j���[�V�����N���b�v
	ModelRender m_modelrender;								//���f�������_�[
	Vector3 m_position;										//���W
	Vector3 m_velocity;										//���x
	Vector3 m_targetPosition;								//�ǔ��Ώۍ��W
	EnShooter m_shooter = enShooter_None;                                    //�ˌ��Ґݒ�
};

