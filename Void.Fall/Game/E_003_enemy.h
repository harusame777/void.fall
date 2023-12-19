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
		enEnemyState_Idle,					//待機
		enEnemyState_Chase,					//追跡
		enEnemyState_Attack,				//攻撃
		enEnemyState_ReceiveDamage,			//被ダメージ。
		enEnemyState_Down,					//ダウン。
	};
	enum EnAnimationClip {
		enAnimationClip_Idle,//棒立ち
		enAnimationClip_Num,//アニメーション数
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
	const bool SearchPlayer() const;                       //プレイヤー探知
	const bool SearchAttackDistance(const Attacktype attacktype) const;               //攻撃距離探知
	void ManageState();//遷移処理
	void ProcessCommonStateTransition();//共通遷移処理
	void ProcessIdleStateTransition();//待機遷移
	//void ProcessChaseStateTransition();//追跡遷移
	//void ProcessAttackStateTransition();//攻撃遷移
	//void ProcessReceiveDamageStateTransition();//被ダメ遷移
	//void ProcessDownStateTransition();//ダウン遷移
	P_main_Player* m_player = nullptr;
	EnEnemyState m_enemystate = enEnemyState_Idle;          //エネミーステート
	Vector3	m_forward = Vector3::AxisZ;						//エネミーの正面ベクトル。
	AnimationClip m_animationclips[enAnimationClip_Num];     //アニメーションクリップ
	enemy_Com m_enemyCom = en_mode_NotCom;
	float m_attackTimer = 5.0f;								//攻撃待機タイマー。
	float attacktime = 5.0f;
};

