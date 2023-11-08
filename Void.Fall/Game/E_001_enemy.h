#pragma once
#include "IEnemy.h"
///////////////////////////////////////////////////////////
class P_main_Player;
class Game;
///////////////////////////////////////////////////////////
class E_001_enemy : public IEnemy   //���N���XIEnemy
{
public:
	//�s���X�e�[�g
	enum EnEnemyState {
		enEnemyState_Idle,					//�ҋ@
		enEnemyState_Chase,					//�ǐ�
		enEnemyState_Attack,				//�U��
		enEnemyState_AttackNear,			//�ߐڍU��
		enEnemyState_ReceiveDamage,			//��_���[�W�B
		enEnemyState_Down,					//�_�E���B
	};
	//�A�j���[�V������/////////////////////////////////////////
	//�A�j���[�V�����X�e�[�g
	enum EnAnimationClip{
		enAnimationClip_Attack,//�U��
		enAnimationClip_AttackNear,//�ߐڍU��
		enAnimationClip_Idle,//�_����
		enAnimationClip_Chase,//�ǐ�
		enAnimationClip_ReceiveDamage,//��_���[�W�B
		enAnimationClip_Down,//�_�E��
		enAnimationClip_Num,//�A�j���[�V������
	};
	//�A�j���[�V�����֐�///////////////////////////////////////
	void PlayAnimation();
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	//�X�e�[�g�J�ڊ֐�/////////////////////////////////////////
	void ManageState();//�J�ڏ���
	void ProcessCommonStateTransition();//���ʑJ�ڏ���
	void ProcessIdleStateTransition();//�ҋ@�J��
	void ProcessChaseStateTransition();//�ǐՑJ��
	void ProcessAttackStateTransition();//�U���J��
	void ProcessReceiveDamageStateTransition();//��_���J��
	void ProcessDownStateTransition();//�_�E���J��
	///////////////////////////////////////////////////////////
	E_001_enemy(){}
	~E_001_enemy() {};
	bool Start();				
	void Update();                                         //�A�b�v�f�[�g
	void Render(RenderContext& rc);                        //���f�������_�[
	void Rotation();                                       //��]
	void Attack();										   //�U��
	void Collision();										//�{�̂̓����蔻��
	void MakeAttackCollision();								//�����蔻��쐬
	const bool SearchPlayer() const;                       //�v���C���[�T�m
	const bool SearchAttackDistance() const;               //�U�������T�m
	const bool SearchAttackDistanceNear() const;               //�U�������T�m(�ߋ���)
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
	//bool IsEnableMove() const				//�ړ��ł��邩�ǂ���
	//{
	//	return m_enemystate != enAnimationClip_Chase &&
	//		m_enemystate != enAnimationClip_Down &&
	//		m_enemystate != enAnimationClip_Idle &&
	//		m_enemystate != enAnimationClip_ReceiveDamage;
	//}
	//�����o�֐��錾
	Game* m_game = nullptr;
	AnimationClip m_animationclips[enAnimationClip_Num];     //�A�j���[�V�����N���b�v
	ModelRender*  m_modelrender = nullptr;					//���f�������_�[
	Vector3 m_scale = Vector3::One;							//�傫��
	Quaternion m_rotation;									//��]
	Vector3 m_movespeed;									//�ړ����x
	Vector3	m_forward = Vector3::AxisZ;						//�G�l�~�[�̐��ʃx�N�g���B
	EnEnemyState m_enemystate = enEnemyState_Idle;          //�G�l�~�[�X�e�[�g
	Vector3 m_targetPosition = Vector3::Zero;				//�ˌ��^�[�Q�b�g
	bool m_isUnderAttack = false;							//�U�������ǂ���(������Ԃ�false�U�����ł͂Ȃ�)
	int m_attacknearboneID = -1;							//�ߐڍU��bone
	int m_hp = 0;                                           //HP
	float m_chaseTimer = 0.0f;						        //�ǐՃ^�C�}�[�B
	float m_idleTimer = 0.0f;							    //�ҋ@�^�C�}�[�B
	float m_attackTimer = 5.0f;								//�U���ҋ@�^�C�}�[�B
};

