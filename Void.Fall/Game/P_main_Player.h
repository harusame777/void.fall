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
		enPlayerState_Avoidance,		//���
		enPlayerState_Down,				//�_�E��
		enPlayerState_ReceiveDamage,    //��_���[�W�B
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
	void Collision();					//�{�̂̓����蔻��
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
		enAnimationClip_ReceiveDamage,  //��_���[�W�B
		enAnimationClip_Down,            //�_�E��
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
	void ProcessReceiveDamageStateTransition();//��_���J��
	void ProcessDownStateTransition();//�_�E���J��
	bool IsEnableMove() const				//�ړ��ł��邩�ǂ���
	{
		return m_playerstate != enPlayerState_Attack &&
			m_playerstate != enPlayerState_Avoidance &&
			m_playerstate != enPlayerState_Idle &&
			m_playerstate != enPlayerState_Walk;
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
	const bool IsLockOn() const
	{
		return m_isLockOn;
	}
	const Vector3& GetTargetPosition() const
	{
		return m_targetPosition;
	}
	//�����o�֐��錾
	std::vector<Vector3*> m_enemyPositionList;
	IEnemy* m_ienemy = nullptr;
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
	SpriteRender m_spriterender;
	Vector3 m_targetPosition = Vector3::Zero;
	Game* m_game = nullptr;
	//�ϐ��錾
	bool m_isTakeAim = false;
	bool m_isLockOn = false;
	int m_numenemy = 0;
	int m_hp = 3;											//HP
	float m_Avoidancetimer = 0.0f;							//����^�C�}�[
	float Avoidancetime = 0.2f;								//�������
	float m_Avoidbreaktimer = 0.0f;							//����N�[���^�C�}�[
	float Avoidbreaktime = 2.0f;							//����N�[���^�C������
	float m_mutekitimer = 0.0f;								//���G�^�C�}�[
	float mutekitime = 2.0f;								//���G����
};

