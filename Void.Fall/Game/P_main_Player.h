#pragma once
class IEnemy;
class Game;
class B_normalbullet;
class P_main_Player : public IGameObject
{
public:
	//�X�e�[�g
	enum EnPlayerState {
		enPlayerState_Idle,				//�ҋ@�B
		enPlayerState_Walk,				//�����B
		enPlayerState_Attack,			//�U��
		enPlayerState_Avoidance			//���
	};
	//�֐��錾
	P_main_Player() {}
	~P_main_Player() {}
	bool Start();
	void Update();						//�A�b�v�f�[�g
	void Render(RenderContext& rc);		//���f�������_�[
	void Move();						//�ړ�
	void Rotation();					//��]
	void ManageState();					//�X�e�[�g�J�ڏ���
	void Lockon();						//���b�N�I��
	void Takeaim();						//���b�N�I���ɑ΂���ʒu�擾
	void Avoidance();					//���
	void AvoidanceTex();				//����X�L���ύX
	void NormalTex();					//�ʏ�X�L���ύX
	//�A�j���[�V������/////////////////////////////////////////
//�A�j���[�V�����X�e�[�g
	enum EnAnimationClip {
		enAnimationClip_Walk,			//����
		enAnimationClip_Attack,			//�U��
		enAnimationClip_Idle,			//�_����
		enAnimationClip_Num				//�A�j���[�V������
	};
	enum EnAnimationClip_sub {
		enAnimationClip_Avoidance,		//���
		enAnimationClip_sub_Num				//�A�j���[�V������
	};
	//�A�j���[�V�����֐�///////////////////////////////////////
	void PlayAnimation();
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	//�X�e�[�g�J�ڊ֐�/////////////////////////////////////////
	void ProcessCommonStateTransition();	//���ʏ���
	void ProcessIdleStateTransition();		//�ҋ@�J��
	void ProcessWalkStateTransition();		//�����J��
	void ProcessAttackStateTransition();	//�U���J��
	void ProcessAvoidanceStateTransition(); //���J��
	bool IsEnableMove() const				//�ړ��ł��邩�ǂ���
	{
		return true;
	}
	///////////////////////////////////////////////////////////
	//�����ݒ�n��
	void Setposition(const Vector3& position)				//���W
	{
		m_position = position;
	}
	const Vector3& Getposition() const						//���W�擾�v���O����
	{
		return m_position;
	}
	void Setrotarion(const Quaternion& rotation)			//��]�l
	{
		m_rotation = rotation;
	}
	void Setscale(const Vector3& scale)						//���W
	{
		m_scale = scale;
	}
	//�����o�֐��錾
	AnimationClip m_animationclips[enAnimationClip_Num];	//�A�j���[�V�����N���b�v
	AnimationClip m_animationclips_sub[enAnimationClip_sub_Num];//�A�j���[�V�����N���b�v
	Vector3 m_position;										//���W
	Vector3 m_scale = Vector3::One;							//�傫��
	Quaternion m_rotation;									//��]
	Vector3 m_movespeed;									//�ړ����x
	Vector3	m_forward = Vector3::AxisZ;						//�v���C���[�̐��ʃx�N�g���B
	EnPlayerState m_playerstate = enPlayerState_Idle;		//�v���C���[�X�e�[�g
	CharacterController	m_charaCon;							//�L�����N�^�[�R���g���[���[�B
	ModelRender* m_modelrender = nullptr;					//���f�������_�[
	Game* m_game = nullptr;
	//�ϐ��錾
	int m_hp = 3;											//HP
	float m_Avoidancetimer = 0.0f;							//����^�C�}�[
	float Avoidancetime = 0.2f;								//�������
	float m_Avoidbreaktimer = 0.0f;							//����N�[���^�C�}�[
	float Avoidbreaktime = 2.0f;							//����N�[���^�C������
};

