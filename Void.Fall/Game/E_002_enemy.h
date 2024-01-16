#pragma once
#include "IEnemy.h"
class P_main_Player;
class E_002_enemy : public IEnemy
{
public:
	//�s���X�e�[�g
	enum EnEnemyState {
		enEnemyState_Idle,					//�ҋ@
		enEnemyState_Chase,					//�ǐ�
		enEnemyState_Attack,				//�U��
		enEnemyState_ReceiveDamage,			//��_���[�W�B
		enEnemyState_Down,					//�_�E���B
	};
	//�A�j���[�V������/////////////////////////////////////////
//�A�j���[�V�����X�e�[�g
	enum EnAnimationClip {
		enAnimationClip_Idle,//�_����
		enAnimationClip_Chase,//�ǐ�
		enAnimationClip_Attack,//�U��
		enAnimationClip_ReceiveDamage,//��_���[�W�B
		enAnimationClip_Down,//�_�E��
		enAnimationClip_Num,//�A�j���[�V������
	};
	void PlayAnimation();
	//�X�e�[�g�J�ڊ֐�/////////////////////////////////////////
	void ManageState();//�J�ڏ���
	void ProcessCommonStateTransition() ;//���ʑJ�ڏ���
	void ProcessIdleStateTransition() ;//�ҋ@�J��
	void ProcessChaseStateTransition() ;//�ǐՑJ��
	void ProcessAttackStateTransition();//�U���J��
	void ProcessReceiveDamageStateTransition();//��_���J��
	void ProcessDownStateTransition();//�_�E���J��
	///////////////////////////////////////////////////////////
	E_002_enemy() {};
	~E_002_enemy() {};
	bool Start();
	void Update();                                         //�A�b�v�f�[�g
	void Rotation();                                       //��]
	void Chase();										   //�ǐ�
	void Attack();										   //�U��
	void Collision();										//�{�̂̓����蔻��
	const bool SearchPlayer() const;                       //�v���C���[�T�m
	const bool SearchAttackDistance() const;               //�U�������T�m
	void Render(RenderContext& rc);                        //���f�������_�[
	bool IsAttack() const				//�ړ��ł��邩�ǂ���
	{
		return m_enemystate != enAnimationClip_Chase &&
			m_enemystate != enAnimationClip_Attack;
	}
	///////////////////////////////////////////////////////////
	//�����ݒ�n��
	//�����o�֐��錾
	E_002_enemy* m_enemy002 = nullptr;
	CollisionObject* m_attackcoll;		//�R���W�����I�u�W�F�N�g�B
	AnimationClip m_animationclips[enAnimationClip_Num];     //�A�j���[�V�����N���b�v
	EnEnemyState m_enemystate = enEnemyState_Idle;          //�G�l�~�[�X�e�[�g
	Vector3	m_forward = Vector3::AxisZ;						//�G�l�~�[�̐��ʃx�N�g���B
	float m_attackTimer = 5.0f;								//�U���ҋ@�^�C�}�[�B
	float attacktime = 5.0f;
	float m_attackGotimer = 0.0;							//�U�����ԃ^�C�}�[
	float attackGotime = 2.0;								//�U������
	float m_attackcooltimer = 0.0f;
	float attackcooltime = 3.0f;
};

