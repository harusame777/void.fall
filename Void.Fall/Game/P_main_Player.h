#pragma once
//#include "IEnemy.h"
//#include "Game.h"
class IEnemy;
class Game;
class B_normalbullet;
class B_homingbullet;

class P_main_Player : public IGameObject
{
public:
	//ステート
	enum EnPlayerState {
		enPlayerState_Idle,				//待機。
		enPlayerState_Walk,				//歩き。
		enPlayerState_Attack,			//攻撃
		enPlayerState_Attacknear,		//近接攻撃
		enPlayerState_Avoidance,		//回避
		enPlayerState_Down,				//ダウン
		enPlayerState_ReceiveDamage,    //被ダメージ。
	};
	enum LockonLRen {
		en_R,
		en_L,
		en_Early
	};
	//関数宣言
	P_main_Player() {}
	~P_main_Player() {}
	bool Start();
	void Update();						//アップデート
	void Render(RenderContext& rc);		//モデルレンダー
	void Move();						//移動
	void Rotation();					//回転
	void Attack();                      //攻撃
	void MakeAttackCollision();			//攻撃当たり判定
	void AttackRotation();				//攻撃時回転
	void ManageState();					//ステート遷移処理
	void Collision();					//本体の当たり判定
	void Lockon();						//ロックオン
	void LockonLR();					//ロックオン変更
	void LockonLRDis(LockonLRen LR);	//ロックオン変更距離計算
	void Takeaim();						//ロックオンに対する位置取得
	void Avoidance();					//回避
	void AvoidanceTex();				//回避スキン変更
	void NormalTex();					//通常スキン変更
	void MPrec();                       //MP処理
	//アニメーション類/////////////////////////////////////////
//アニメーションステート
	enum EnAnimationClip {
		enAnimationClip_Walk,			//歩き
		enAnimationClip_Attack,			//攻撃
		enAnimationClip_Attacknear,		//近接攻撃
		enAnimationClip_Idle,			//棒立ち
		enAnimationClip_ReceiveDamage,  //被ダメージ。
		enAnimationClip_Down,            //ダウン
		enAnimationClip_Num				//アニメーション数
	};
	enum EnAnimationClip_sub {
		enAnimationClip_Avoidance,		//回避
		enAnimationClip_sub_Num				//アニメーション数
	};
	//アニメーション関数///////////////////////////////////////
	void PlayAnimation();
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	//ステート遷移関数/////////////////////////////////////////
	void ProcessCommonStateTransition();	//共通処理
	void ProcessIdleStateTransition();		//待機遷移
	void ProcessWalkStateTransition();		//歩き遷移
	void ProcessAttackStateTransition();	//攻撃遷移
	void ProcessAvoidanceStateTransition(); //回避遷移
	void ProcessReceiveDamageStateTransition();//被ダメ遷移
	void ProcessDownStateTransition();//ダウン遷移
	void RockOnPositionCheck();

	bool IsEnableMove() const				//移動できるかどうか
	{
		return m_playerstate != enPlayerState_Attack &&
			m_playerstate != enPlayerState_Attacknear && 
			m_playerstate != enPlayerState_Avoidance &&
			m_playerstate != enPlayerState_Idle &&
			m_playerstate != enPlayerState_Walk;
	}
	///////////////////////////////////////////////////////////
	//初期設定系統
	void Setposition(const Vector3& position)				//座標
	{
		m_position = position;
	}
	const Vector3& Getposition() const						//座標取得プログラム
	{
		return m_position;
	}
	void Setrotarion(const Quaternion& rotation)			//回転値
	{
		m_rotation = rotation;
	}
	void Setscale(const Vector3& scale)						//座標
	{
		m_scale = scale;
	}
	const bool IsLockOn() const				
	{
		return m_isLockOn;
	}
	const Vector3& GetTargetPosition() const
	{
		return m_targetPosition;
	}
	///////////////////////////////////////////////////////////
	//計算、または省略した関数
	//MP消費(引数はコスト)
	void MP_Re(int mpcost)
	{
		m_mp -= mpcost;
		m_mpRec = mpRecReset;
		mpRecgo = true;
	}
	//正面に引数１があるかどうかboolで返すあったらtrue(引数１はenemyのポジション、
	//引数２はplayerのポジション)
	bool AngleCheck(const Vector3& positionE
		,const Vector3& positionP)
	{
		Vector3 diff = positionE - positionP;
		diff.Normalize();
		float angle = acosf(m_forward.Dot(diff));
		if (angle > Math::PI / 2){
			return false;
		}
		else{
			return true;
		}
	}
	//現在のステートがAttacknearかどうか調べる関数(Attacknearだったらfalseが帰ってくる)
	bool AttackF()
	{
		if (m_playerstate == enPlayerState_Attacknear){
			return false;
		}
		else{
			return true;
		}
	}
	void LockOnFalse()
	{
		enemypossub = { 1000.0f,1000.0f,1000.0f };
		m_isTakeAim = false;
	}
	///////////////////////////////////////////////////////////
	//メンバ関数宣言
	CollisionObject* m_collisionObject;						//コリジョンオブジェクト。
	std::vector<Vector3*> m_enemyPositionList;              //エネミー座標動的配列
	IEnemy* m_ienemy = nullptr;								//エネミー基底クラス
	AnimationClip m_animationclips[enAnimationClip_Num];	//アニメーションクリップ
	AnimationClip m_animationclips_sub[enAnimationClip_sub_Num];//アニメーションクリップ
	Vector3 m_position;										//座標
	Vector3 m_scale = Vector3::One;							//大きさ
	Quaternion m_rotation;									//回転
	Vector3 m_movespeed;									//移動速度
	Vector3	m_forward = Vector3::AxisZ;						//プレイヤーの正面ベクトル。
	EnPlayerState m_playerstate = enPlayerState_Idle;		//プレイヤーステート
	CharacterController	m_charaCon;							//キャラクターコントローラー。
	ModelRender* m_modelrender = nullptr;					//モデルレンダー
	SpriteRender m_spriterender;							//スプライトレンダー
	Vector3 m_targetPosition = Vector3::Zero;				//ロックオン対象座標
	Game* m_game = nullptr;									//ゲーム
	Vector3 enemypossub = { 1000.0f,1000.0f,1000.0f };		//ロックオン計算に使うヤツ
	//変数宣言
	bool m_isTakeAim = false;								//ロックオンの位置取れてるか
	bool m_isLockOn = false;								//ロックオンtrue:false
	LockonLRen LR = en_Early;								//ロックオン変更用
	int m_numenemy = 0;										//現在エネミーの数
	int ListnumB = 0;										//リストナンバーB(ロックオン時固定)
	int ListnumA = 0;										//リストナンバーA(ロックオン計算時)
	int m_hp = 3;											//HP
	int m_mp = 3;											//MP
	float m_Avoidancetimer = 0.0f;							//回避タイマー
	float Avoidancetime = 0.2f;								//回避時間
	float m_Avoidbreaktimer = 0.0f;							//回避クールタイマー
	float Avoidbreaktime = 2.0f;							//回避クールタイム時間
	float m_mutekitimer = 0.0f;								//無敵タイマー
	float mutekitime = 2.0f;								//無敵時間
	float m_mpRec = 0.0f;									//mp回復タイマー
	float mpRecReset = 0.0f;								//mpタイマーリセット
	float m_attacktime = 0.0f;								//攻撃クールダウンタイマー
	float attacktimer = 0.3f;								//攻撃クールダウン時間
 	bool mpRecgo = false;									//mp回復するかしないか
	bool m_isUnderAttack = false;							//現在攻撃判定中か
	bool m_attack1time = false;
	Vector3 enemy;
};

