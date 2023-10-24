#pragma once
class IEnemy;
class P_main_Player;
class B_normalbullet : public IGameObject
{
///////////////////////////////////////////////////////////
public:
	B_normalbullet() {};
	~B_normalbullet() {};
	bool Start();
	void Update();
	void Rotation();
	void Movebullet();
	void Render(RenderContext& rc);
	void Inpacttime();
	void Inpacthit();
	void Delay(); //�e�ۏ����f�B���C
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
	void Setrotation(const Quaternion& rotation)//��]�l
	{
		m_rotation = rotation;
	}
//�����o�֐��錾
	float bullettime = 1.5f;
	float bullettdelaytime = 2.0f;
	AnimationClip m_animationclips[enAnimationClip_Num];    //�A�j���[�V�����N���b�v
	Vector3	m_forward = Vector3::AxisZ;						//�G�l�~�[�̐��ʃx�N�g���B
	ModelRender* m_modelrender = nullptr;					//���f�������_�[
	CollisionObject* m_collisionObject;						//�R���W�����I�u�W�F�N�g�B
	P_main_Player* m_player = nullptr;                      //�v���C���[
	IEnemy* m_ienemy = nullptr;
	Quaternion	m_rotation;									//��]�B
	Vector3 m_position;										//���W
	Vector3 m_velocity;										//���x
	Vector3	m_scale = Vector3::One;							//�傫���B
};

