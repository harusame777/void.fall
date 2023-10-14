#pragma once
class P_main_Player : public IGameObject
{
public:
	//�X�e�[�g
	enum EnPlayerState {
		enPlayerState_Idle,//�ҋ@�B
		enPlayerState_Walk,//�����B
		enPlayerState_Idlefry,//�z�o�[�ҋ@�B
		enPlayerState_Walkfry,//�z�o�[�����B
	};
	//�֐��錾
	P_main_Player() {}
	~P_main_Player() {}
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void Move();//�ړ�
	void Movefry();//�z�o�[
	void Rotation();//��]
	void ManageState();//�X�e�[�g�J�ڏ���
	//�A�j���[�V������/////////////////////////////////////////
//�A�j���[�V�����X�e�[�g
	enum EnAnimationClip {
		enAnimationClip_Walk,//����
		//enAnimationClip_Attack,//�U��
		enAnimationClip_Idle,//�_����
		enAnimationClip_Num//�A�j���[�V������
	};
	//�A�j���[�V�����֐�///////////////////////////////////////
	void PlayAnimation();
	//�X�e�[�g�J�ڊ֐�/////////////////////////////////////////
	void ProcessCommonStateTransition();//���ʏ���
	void ProcessIdleStateTransition();//�ҋ@�J��
	void ProcessWalkStateTransition();//�����J��
	bool IsEnableMove() const//�ʏ�ړ���Ԃ��̊m�F
	{
		return m_playerstate != enPlayerState_Idlefry &&
			m_playerstate != enPlayerState_Walkfry;
	}
	bool IsEnableMovefry() const//�z�o�[��Ԃ��̊m�F
	{
		return m_playerstate != enPlayerState_Idle &&
			m_playerstate != enPlayerState_Walk;
	}
	///////////////////////////////////////////////////////////
	//�����ݒ�n��
	void Setposition(const Vector3& position)//���W
	{
		m_position = position;
	}
	const Vector3& Getposition() const//���W�擾�v���O����
	{
		return m_position;
	}
	void Setrotarion(const Quaternion& rotation)//��]�l
	{
		m_rotation = rotation;
	}
	void Setscale(const Vector3& scale)//���W
	{
		m_scale = scale;
	}
	//�����o�֐��錾
	AnimationClip m_animationclips[enAnimationClip_Num];     //�A�j���[�V�����N���b�v
	Vector3 m_position;										//���W
	Vector3 m_scale = Vector3::One;							//�傫��
	Quaternion m_rotation;									//��]
	Vector3 m_movespeed;									//�ړ����x
	Vector3	m_forward = Vector3::AxisZ;						//�v���C���[�̐��ʃx�N�g���B
	EnPlayerState m_playerstate = enPlayerState_Idle;		//�v���C���[�X�e�[�g
	CharacterController	m_charaCon;							//�L�����N�^�[�R���g���[���[�B
	ModelRender* m_modelrender = nullptr;					//���f�������_�[
	//�ϐ��錾
	int m_hp = 3;											//HP
	bool m_fryflag = false;									//�z�o�[�t���O1.true��ׂ�||0.false��ׂȂ�
	int m_fry = 100;										//�z�o�[�Q�[�W
	int m_frymax = 100;										//�z�o�[�Q�[�W�ő�
	float timerfry = 0.0f;
	void Addfry()
	{
		if (IsEnableMovefry() == false)
		{
			if (timerfry > 0.0f)
			{
				return;
			}
			if (m_fry > 100)
			{
				return;
			}
			m_fry++;
			timerfry = 0.1f;
		}
		else if (IsEnableMove() == false)
		{
			if (timerfry > 0.0f)
			{
				return;
			}
			m_fry--;
			timerfry = 0.1f;
		}
	}
};

