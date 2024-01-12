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
		enEnemyState_Standby,					//待機状態
		enEnemyState_Active,					//起動状態
		enEnemyState_Down,					    //ダウン。
	};
	enum EnAnimationClip {
		enAnimationClip_Standby,//棒立ち
		enAnimationClip_Active,
		enAnimationClip_Down,
		enAnimationClip_Num,//アニメーション数
	};
	void PlayAnimation();
	bool Start();	
	void Collision();										//本体の当たり判定
	void Update();
	void Render(RenderContext& rc);
	void ManageState();//遷移処理
	void ProcessDownStateTransition();//ダウン遷移
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
	Vector3	m_forward = Vector3::AxisZ;						//エネミーの正面ベクトル。
	EnEnemyState m_enemystate = enEnemyState_Standby;          //エネミーステート
	AnimationClip m_animationclips[enAnimationClip_Num];     //アニメーションクリップ
	M_parts4* m_parts4 = nullptr;
	M_parts4_sub* m_parts4_sub = nullptr;
	int map_num = 0;
	float m_Locktimer = 0.0f;
	float Locktime = 60.0f;
	bool EnemyOneSum = false;
};

