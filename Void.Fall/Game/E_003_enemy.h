#pragma once
#include "IEnemy.h"
class P_main_Player;
class E_003_enemy : public IEnemy
{
public:
	enum enemy_Com{
		en_mode_NotCom,
		en_mode_ComMain,
		en_mode_ComSub,
	};
	enum EnEnemyState {
		enEnemyState_Idle,					//�ҋ@
		enEnemyState_Chase,					//�ǐ�
		enEnemyState_Attack,				//�U��
		enEnemyState_ReceiveDamage,			//��_���[�W�B
		enEnemyState_Down,					//�_�E���B
	};
	enum EnAnimationClip {
		enAnimationClip_Idle,//�_����
		enAnimationClip_Num,//�A�j���[�V������
	};
	enum Attacktype {
		en_none,
		en_attackN,
		en_attackF,
	};
	void PlayAnimation();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	const bool SearchPlayer() const;                       //�v���C���[�T�m
	const bool SearchAttackDistance(const Attacktype attacktype) const;               //�U�������T�m
	void ManageState();//�J�ڏ���
	void ProcessCommonStateTransition();//���ʑJ�ڏ���
	void ProcessIdleStateTransition();//�ҋ@�J��
	//void ProcessChaseStateTransition();//�ǐՑJ��
	//void ProcessAttackStateTransition();//�U���J��
	//void ProcessReceiveDamageStateTransition();//��_���J��
	//void ProcessDownStateTransition();//�_�E���J��
	P_main_Player* m_player = nullptr;
	EnEnemyState m_enemystate = enEnemyState_Idle;          //�G�l�~�[�X�e�[�g
	Vector3	m_forward = Vector3::AxisZ;						//�G�l�~�[�̐��ʃx�N�g���B
	AnimationClip m_animationclips[enAnimationClip_Num];     //�A�j���[�V�����N���b�v
	enemy_Com m_enemyCom = en_mode_NotCom;
	float m_attackTimer = 5.0f;								//�U���ҋ@�^�C�}�[�B
	float attacktime = 5.0f;
};

