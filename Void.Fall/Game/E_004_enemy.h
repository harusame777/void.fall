#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "IEnemy.h"
class P_main_Player;
class M_parts4;
class M_parts4_sub;
class IEnemy;
class E_004_enemy : public IEnemy
{
public :
	enum EnEnemyState {
		enEnemyState_Standby,					//�ҋ@���
		enEnemyState_Active,					//�N�����
		enEnemyState_Down,					    //�_�E���B
	};
	enum EnAnimationClip {
		enAnimationClip_Standby,//�_����
		enAnimationClip_Active,
		enAnimationClip_Down,
		enAnimationClip_Num,//�A�j���[�V������
	};
	void PlayAnimation();
	bool Start();	
	void Collision();										//�{�̂̓����蔻��
	void Update();
	void Render(RenderContext& rc);
	void ManageState();//�J�ڏ���
	void ProcessDownStateTransition();//�_�E���J��
	void ActiveLock();
	void EnemySummon();
	void EnemyRand(/*int randnum,*/int Vecnum);
	Vector3 EnemySetVec(int Vecnum);
	void Setnum(int num) {
		map_num = num;
	}
	void Standby(){
		m_enemystate = enEnemyState_Standby;
		EnemyOneSum = false;
		QueryGOs<IEnemy>("summonenemy", [&](IEnemy* ienemy) {
			DeleteGO(ienemy);
			return true;
		});
	}
	int Random()
	{
		srand((unsigned int)time(NULL));
		int enemyrand = rand() % 2 + 1;
		return enemyrand;
	}
	Vector3	m_forward = Vector3::AxisZ;						//�G�l�~�[�̐��ʃx�N�g���B
	EnEnemyState m_enemystate = enEnemyState_Standby;          //�G�l�~�[�X�e�[�g
	AnimationClip m_animationclips[enAnimationClip_Num];     //�A�j���[�V�����N���b�v
	M_parts4* m_parts4 = nullptr;
	M_parts4_sub* m_parts4_sub = nullptr;
	int map_num = 0;
	float m_Locktimer = 0.0f;
	float Locktime = 60.0f;
	bool EnemyOneSum = false;
};

