#pragma once
///////////////////////////////////////////////////////////
class P_main_Player;
///////////////////////////////////////////////////////////
class E_001_enemy : public IGameObject
{
public:
	//�s���X�e�[�g
	enum EnEnemyState {
		enEnemyState_Idle,//�ҋ@
		enEnemyState_Chase,//�ǐ�
		enEnemyState_Attack//�U��
	};
	//�A�j���[�V������/////////////////////////////////////////
	//�A�j���[�V�����X�e�[�g
	enum EnAnimationClip{
		enAnimationClip_Walk,//����
		//enAnimationClip_Attack,//�U��
		//enAnimationClip_Idle,//�_����
		enAnimationClip_Num//�A�j���[�V������
	};
	//�A�j���[�V�����֐�///////////////////////////////////////
	//�X�e�[�g�J�ڊ֐�/////////////////////////////////////////
	void ManageState();//�J�ڏ���
	void ProcessCommonStateTransition();//���ʑJ�ڏ���
	void ProcessIdleStateTransition();//�ҋ@�J��
	void ProcessChaseStateTransition();//�ǐՑJ��
	//void ProcessAttackStateTransition();//�U���J��
	void PlayAnimation();
	///////////////////////////////////////////////////////////
	E_001_enemy(){}
	~E_001_enemy(){}
	bool Start();				
	void Update();                                         //�A�b�v�f�[�g
	void Render(RenderContext& rc);                        //���f�������_�[
	void Chase();                                          //�ǐ�
	void Rotation();                                       //��]
	void Attack();										   //�U��
	const bool SearchPlayer() const;                       //�v���C���[�T�m
	const bool SearchAttackDistance() const;               //�U�������T�m
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
	void Setrotation(const Quaternion& rotation)//��]�l
	{
		m_rotation = rotation;
	}
	void Setscale(const Vector3& scale)//���W
	{
		m_scale = scale;
	}
	void SetHP(const int hp)//����HP
	{
		m_hp = hp;
	}
	//�����o�֐��錾
	AnimationClip m_animationclips[enAnimationClip_Num];     //�A�j���[�V�����N���b�v
	ModelRender*  m_modelrender = nullptr;								//���f�������_�[
	Vector3 m_position;										//���W
	Vector3 m_scale = Vector3::One;							//�傫��
	Quaternion m_rotation;									//��]
	Vector3 m_movespeed;									//�ړ����x
	Vector3	m_forward = Vector3::AxisZ;						//�G�l�~�[�̐��ʃx�N�g���B
	EnEnemyState m_enemystate = enEnemyState_Idle;          //�G�l�~�[�X�e�[�g
	CharacterController	m_charaCon;							//�L�����N�^�[�R���g���[���[�B
	P_main_Player* m_player = nullptr;                      //�v���C���[
	bool m_isUnderAttack = false;							//�U�������ǂ���(������Ԃ�false�U�����ł͂Ȃ�)
	int m_hp = 0;                                           //HP
	float m_chaseTimer = 0.0f;						        //�ǐՃ^�C�}�[�B
	float m_idleTimer = 0.0f;							    //�ҋ@�^�C�}�[�B
};

