#pragma once
#include "IEnemy.h"
class P_main_Player;
class M_parts4;
class E_004_enemy : public IEnemy
{
public :
	enum EnEnemyState {
		enEnemyState_Standby,					//待機状態
		enEnemyState_Active,					//起動状態
	};
	enum EnAnimationClip {
		enAnimationClip_Standby,//棒立ち
		enAnimationClip_Num,//アニメーション数
	};
	void PlayAnimation();
	bool Start();	
	void Collision();										//本体の当たり判定
	void Update();
	void Render(RenderContext& rc);
	void ActiveLock();
	void EnemySummon();
	void Setnum(int num) {
		map_num = num;
	}
	Vector3	m_forward = Vector3::AxisZ;						//エネミーの正面ベクトル。
	EnEnemyState m_enemystate = enEnemyState_Standby;          //エネミーステート
	AnimationClip m_animationclips[enAnimationClip_Num];     //アニメーションクリップ
	M_parts4* m_parts4 = nullptr;
	int map_num = 0;
	float m_Locktimer = 0.0f;
	float Locktime = 60.0f;
};

