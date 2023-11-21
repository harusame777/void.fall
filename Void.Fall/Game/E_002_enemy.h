#pragma once
#include "IEnemy.h"
class P_main_Player;
class E_002_enemy : public IEnemy
{
public:
	//行動ステート
	enum EnEnemyState {
		enEnemyState_Idle,					//待機
		enEnemyState_Chase,					//追跡
		enEnemyState_Attack,				//攻撃
	};
	//アニメーション類/////////////////////////////////////////
//アニメーションステート
	enum EnAnimationClip {
		enAnimationClip_Idle,//棒立ち
		enAnimationClip_Chase,//追跡
		enAnimationClip_Attack,//攻撃
		enAnimationClip_Num,//アニメーション数
	};
	void PlayAnimation();
	//ステート遷移関数/////////////////////////////////////////
	void ManageState();//遷移処理
	void ProcessCommonStateTransition() ;//共通遷移処理
	void ProcessIdleStateTransition() ;//待機遷移
	void ProcessChaseStateTransition() ;//追跡遷移
	void ProcessAttackStateTransition();//攻撃遷移
	///////////////////////////////////////////////////////////
	E_002_enemy() {};
	~E_002_enemy() {};
	bool Start();
	void Update();                                         //アップデート
	void Rotation();                                       //回転
	void Chase();										   //追跡
	const bool SearchPlayer() const;                       //プレイヤー探知
	const bool SearchAttackDistance() const;               //攻撃距離探知
	void Render(RenderContext& rc);                        //モデルレンダー
	///////////////////////////////////////////////////////////
	//初期設定系統
	//メンバ関数宣言
	E_002_enemy* m_enemy002 = nullptr;
	AnimationClip m_animationclips[enAnimationClip_Num];     //アニメーションクリップ
	EnEnemyState m_enemystate = enEnemyState_Idle;          //エネミーステート
	Vector3	m_forward = Vector3::AxisZ;						//エネミーの正面ベクトル。
	float m_attackTimer = 5.0f;								//攻撃待機タイマー。
	float attacktime = 5.0f;
};

