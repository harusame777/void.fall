#pragma once
#include "IEnemy.h"
///////////////////////////////////////////////////////////
class P_main_Player;
///////////////////////////////////////////////////////////
class E_001_enemy : public IEnemy
{
public:
	//行動ステート
	enum EnEnemyState {
		enEnemyState_Idle,					//待機
		enEnemyState_Chase,					//追跡
		enEnemyState_Attack,				//攻撃
		enEnemyState_AttackNear,			//近接攻撃
		enEnemyState_ReceiveDamage,			//被ダメージ。
		enEnemyState_Down,					//ダウン。
	};
	//アニメーション類/////////////////////////////////////////
	//アニメーションステート
	enum EnAnimationClip{
		enAnimationClip_Attack,//攻撃
		enAnimationClip_AttackNear,//近接攻撃
		enAnimationClip_Idle,//棒立ち
		enAnimationClip_Chase,//追跡
		enAnimationClip_Num//アニメーション数
	};
	//アニメーション関数///////////////////////////////////////
	void PlayAnimation();
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	//ステート遷移関数/////////////////////////////////////////
	void ManageState();//遷移処理
	void ProcessCommonStateTransition();//共通遷移処理
	void ProcessIdleStateTransition();//待機遷移
	void ProcessChaseStateTransition();//追跡遷移
	void ProcessAttackStateTransition();//攻撃遷移
	///////////////////////////////////////////////////////////
	E_001_enemy(){}
	bool Start();				
	void Update();                                         //アップデート
	void Render(RenderContext& rc);                        //モデルレンダー
	void Rotation();                                       //回転
	void Attack();										   //攻撃
	void Collision();										//本体の当たり判定
	void MakeAttackCollision();								//当たり判定作成
	const bool SearchPlayer() const;                       //プレイヤー探知
	const bool SearchAttackDistance() const;               //攻撃距離探知
	const bool SearchAttackDistanceNear() const;               //攻撃距離探知(近距離)
	///////////////////////////////////////////////////////////
	//初期設定系統
	void Setposition(const Vector3& position)//座標
	{
		m_position = position;
	}
	const Vector3& Getposition() const//座標取得プログラム
	{
		return m_position;
	}
	void Setrotation(const Quaternion& rotation)//回転値
	{
		m_rotation = rotation;
	}
	void Setscale(const Vector3& scale)//座標
	{
		m_scale = scale;
	}
	void SetHP(const int hp)//初期HP
	{
		m_hp = hp;
	}
	void Setmobtype(const bool mobs)//モブタイプ
	{
		if (mobs)
			mobty = enenemy_M;
		else
			mobty = enplayer_M;
	}
	//メンバ関数宣言
	AnimationClip m_animationclips[enAnimationClip_Num];     //アニメーションクリップ
	ModelRender*  m_modelrender = nullptr;					//モデルレンダー
	Vector3 m_position;										//座標
	Vector3 m_scale = Vector3::One;							//大きさ
	Quaternion m_rotation;									//回転
	Vector3 m_movespeed;									//移動速度
	Vector3	m_forward = Vector3::AxisZ;						//エネミーの正面ベクトル。
	EnEnemyState m_enemystate = enEnemyState_Idle;          //エネミーステート
	CharacterController	m_charaCon;							//キャラクターコントローラー。
	P_main_Player* m_player = nullptr;                      //プレイヤー
	Vector3 m_targetPosition = Vector3::Zero;				//射撃ターゲット
	bool m_isUnderAttack = false;							//攻撃中かどうか(初期状態はfalse攻撃中ではない)
	int m_attacknearboneID = -1;							//近接攻撃bone
	int m_hp = 0;                                           //HP
	float m_chaseTimer = 0.0f;						        //追跡タイマー。
	float m_idleTimer = 0.0f;							    //待機タイマー。
	float m_attackTimer = 5.0f;								//攻撃待機タイマー。
	enum mobtype{
		enplayer_M,
		enenemy_M
	};
	mobtype mobty = enenemy_M;
};

