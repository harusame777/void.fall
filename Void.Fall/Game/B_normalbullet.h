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
	void Update();						//�A�b�v�f�[�g
	void Rotation();					//��]
	void Movebullet();					//�e�ۈړ�
	void Render(RenderContext& rc);		//���f�������_�[
	void Inpacttime();					//�e�ێ��R����
	void Inpacthit();					//�e�ۏՓ�
	void deletebullet();				//�e�ۏ���
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
	void SetVelocity(const Vector3& velocity)//���x�����ݒ�
	{
		m_velocity = velocity;
	}
	void SetPosition(const Vector3& position)//�ʒu�����ݒ�
	{
		m_position = position;
	}
	void Setrotation(const Quaternion& rotation)//��]�l�����ݒ�
	{
		m_rotation = rotation;
	}
//�����o�֐��錾
	float bullettime = 1.5f;								//���R�����^�C�}�[
	float m_deleteTimer = 0.0f;								//�����f�B���C
	bool m_isDelete = false;								//�����f�B���C�Ɏg�����
	const float deletetimer = 0.1f;							//�f�B���C�^�C�}�[
	AnimationClip m_animationclips[enAnimationClip_Num];    //�A�j���[�V�����N���b�v
	Vector3	m_forward = Vector3::AxisZ;						//�G�l�~�[�̐��ʃx�N�g���B
	ModelRender* m_modelrender = nullptr;					//���f�������_�[
	CollisionObject* m_collisionObject;						//�R���W�����I�u�W�F�N�g�B
	P_main_Player* m_player = nullptr;                      //�v���C���[
	Quaternion	m_rotation;									//��]�B
	Vector3 m_position;										//���W
	Vector3 m_velocity;										//���x
	Vector3	m_scale = Vector3::One;							//�傫���B
};

