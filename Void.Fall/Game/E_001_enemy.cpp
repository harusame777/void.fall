#include "stdafx.h"
#include "E_001_enemy.h"
#include "P_main_Player.h"
#include "collision/CollisionObject.h"
#include "B_homingbullet.h"
#include "Game.h"
#include "graphics/effect/EffectEmitter.h"
///////////////////////////////////////////////////////////
#include <time.h>
#include <stdlib.h>
///////////////////////////////////////////////////////////
#define enemyspeed 100.0f                               //移動速度の数値
#define enemyserch 700.0f * 700.0f						//追跡可能範囲
#define enemyserchnear 200.0f * 200.0f					//追跡可能範囲(近接)
#define attacktime 5.0f									//アタックタイマー
namespace
{
	const Vector3 corre1 = { 0.0f,60.0f,0.0f };//位置修正本体当たり判定
	const Vector3 corre2 = { 0.0f,80.0f,10.0f };//位置修正弾丸発生位置
}

bool E_001_enemy::Start()
{

	//アニメーション読み込み
	m_animationclips[enAnimationClip_Idle].Load("Assets/animData/Enemy/enemy_001/idle.tka");
	m_animationclips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationclips[enAnimationClip_Chase].Load("Assets/animData/Enemy/enemy_001/chase.tka");
	m_animationclips[enAnimationClip_Chase].SetLoopFlag(true);
	//enAnimationClip_Attack:アニメーションキーname(attack_point)
	m_animationclips[enAnimationClip_Attack].Load("Assets/animData/Enemy/enemy_001/attack.tka");
	m_animationclips[enAnimationClip_Attack].SetLoopFlag(false);
	//enAnimationClip_AttackNear:アニメーションキーname(attack_start)
	//enAnimationClip_AttackNear:アニメーションキーname(attack_end)
	//bone情報(sub1)
	m_animationclips[enAnimationClip_AttackNear].Load("Assets/animData/Enemy/enemy_001/attacknear.tka");
	m_animationclips[enAnimationClip_AttackNear].SetLoopFlag(false);
	m_animationclips[enAnimationClip_ReceiveDamage].Load("Assets/animData/Enemy/enemy_001/receivedamage.tka");
	m_animationclips[enAnimationClip_ReceiveDamage].SetLoopFlag(false);
	m_animationclips[enAnimationClip_Down].Load("Assets/animData/Enemy/enemy_001/down.tka");
	m_animationclips[enAnimationClip_ReceiveDamage].SetLoopFlag(false);

	//エフェクト読み込み

	//モデル読み込み
	m_modelrender.Init("Assets/modelData/Enemy/enemy_001/RE_enemy_001.tkm",m_animationclips, enAnimationClip_Num);

	//剣のボーンのIDを取得する。
	m_attacknearboneID = m_modelrender.FindBoneID(L"sub1");

	//アニメーションイベント用関数設定
	m_modelrender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	//回転
	m_modelrender.SetRotation(m_rotation);
	//座標
	m_modelrender.SetPosition(m_position);
	//スケール
	m_modelrender.SetScale(m_scale);
	//キャラコン初期化
	m_charaCon.Init(20.0f,100.0f,m_position);

	//コリジョンオブジェクトを作成する。
	m_collisionObject = NewGO<CollisionObject>(0);
	//球状のコリジョンを作成する。
	m_collisionObject->CreateSphere(m_position, Quaternion::Identity, 60.0f * m_scale.z);
	m_collisionObject->SetName("enemy_col");
	m_collisionObject->SetPosition(m_position + corre1);
	//コリジョンオブジェクトが自動で削除されないようにする。
	m_collisionObject->SetIsEnableAutoDelete(false);

	m_player = FindGO<P_main_Player>("player");
	m_game = FindGO<Game>("game");
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);

	EnemyGoEffect();
	Sheldeffect();

	return true;
}

void E_001_enemy::Update()
{
	//回転処理
	Rotation();
	//攻撃処理
	Attack();
	//当たり判定処理
	Collision();
	//アニメーション
	PlayAnimation();
	//ステート遷移処理
	ManageState();
	//描画更新
	m_modelrender.Update();
}

void E_001_enemy::Rotation()
{
	if (fabsf(m_movespeed.x) < 0.001f
		&& fabsf(m_movespeed.z) < 0.001f) {
		//m_moveSpeed.xとm_moveSpeed.zの絶対値がともに0.001以下ということは
		//このフレームではキャラは移動していないので旋回する必要はない。
		return;
	}
	//atan2はtanθの値を角度(ラジアン単位)に変換してくれる関数。
	//m_moveSpeed.x / m_moveSpeed.zの結果はtanθになる。
	//atan2を使用して、角度を求めている。
	//これが回転角度になる。
	float angle = atan2(-m_movespeed.x, m_movespeed.z);
	//atanが返してくる角度はラジアン単位なので
	//SetRotationDegではなくSetRotationを使用する。
	m_rotation.SetRotationY(-angle);

	//回転を設定する。
	m_modelrender.SetRotation(m_rotation);
	m_collisionObject->SetRotation(m_rotation);
	m_collisionObject->SetPosition(m_position + corre1);

	//プレイヤーの前ベクトルを計算する。
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}

void E_001_enemy::Collision()
{
	//被ダメージ、あるいはダウンステートの時は。
//当たり判定処理はしない。
	if (m_enemystate == enEnemyState_ReceiveDamage ||
		m_enemystate == enEnemyState_Down)
	{
		return;
	}
	//プレイヤーの攻撃用のコリジョンを取得する。
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_attack");
	//コリジョンの配列をfor文で回す。
	for (auto collision : collisions)
	{
		//コリジョンとキャラコンが衝突したら。
		if (collision->IsHit(m_collisionObject))
		{
			SoundSource* se = NewGO<SoundSource>(5);
			se = NewGO<SoundSource>(5);
			se->Init(5);
			se->Play(false);
			if (m_sh > 0){
				m_sh--;
				//被ダメージステートに遷移する。
				m_enemystate = enEnemyState_ReceiveDamage;
				return;
			}
			else
			{
				//HPを1減らす。
				m_hp -= 1;
				//HPが0になったら。
				if (m_hp == 0) {
					//ダウンステートに遷移する。
					m_enemystate = enEnemyState_Down;
				}
				else {
					//被ダメージステートに遷移する。
					m_enemystate = enEnemyState_ReceiveDamage;
				}
			}
		}
	}
}

const bool E_001_enemy::SearchPlayer() const
{
	Vector3 diff = m_player->Getposition() - m_position;

	//プレイヤーにある程度近かったら.。
	if (diff.LengthSq() <= enemyserch)
	{
			return true;
	}
	//プレイヤーを見つけられなかった。
	return false;
}

const bool E_001_enemy::SearchAttackDistance() const
{
	Vector3 diff = m_player->Getposition() - m_position;
	//プレイヤーにある程度近かったら.。

	if (diff.LengthSq() <= enemyserch)
	{
		//プレイヤーが射程圏内に入った！
		return true;
	}
	//プレイヤーが射程圏外だった。
	return false;
}

const bool E_001_enemy::SearchAttackDistanceNear() const
{
	Vector3 diff2 = m_player->Getposition() - m_position;
	//プレイヤーにある程度近かったら.。

	if (diff2.LengthSq() <= enemyserchnear)
	{
		//プレイヤーが射程圏内に入った！
		return true;
	}
	//プレイヤーが射程圏外だった。
	return false;
}

void E_001_enemy::Attack()
{
	if (m_enemystate != enEnemyState_AttackNear)
	{
		return;
	}
	
	if (m_isUnderAttack == true)
	{
		//当たり判定のとこ
		MakeAttackCollision();
	}
}

void E_001_enemy::MakeAttackCollision()
{
	//攻撃当たり判定用のコリジョンオブジェクトを作成する。
	auto collisionObject = NewGO<CollisionObject>(0);
	//剣のボーンのワールド行列を取得する。
	Matrix matrix = m_modelrender.GetBone(m_attacknearboneID)->GetWorldMatrix();
	//ボックス状のコリジョンを作成する。
	collisionObject->CreateBox(m_position, Quaternion::Identity, Vector3(100.0f, 10.0f, 10.0f));
	collisionObject->SetWorldMatrix(matrix);
	collisionObject->SetName("enemy_attack");
}

void E_001_enemy::ManageState()
{
	switch (m_enemystate)
	{
	//待機
	case enEnemyState_Idle:
		//待機ステート遷移
		ProcessIdleStateTransition();
		break;
    //追跡
	case enEnemyState_Chase:
		//追跡ステート遷移
		ProcessChaseStateTransition();
		break;
	case enEnemyState_Attack:
	case enEnemyState_AttackNear:
		//攻撃ステート遷移
		ProcessAttackStateTransition();
		break;
	case enEnemyState_ReceiveDamage:
		//被ダメステート遷移
		ProcessReceiveDamageStateTransition();
		break;
	case enEnemyState_Down:
		//ダウンステート遷移
		ProcessDownStateTransition();
		break;
	}
}

void E_001_enemy::PlayAnimation()
{
	m_modelrender.SetAnimationSpeed(1.0f);
	switch (m_enemystate)
	{
	//待機
	case enEnemyState_Idle:
		m_modelrender.PlayAnimation(enAnimationClip_Idle, 0.1f);
		break;
	case enEnemyState_Chase:
		m_modelrender.PlayAnimation(enAnimationClip_Chase, 0.1f);
		break;
	case enEnemyState_Attack:
		m_modelrender.PlayAnimation(enAnimationClip_Attack, 0.1f);
		break;
	case enEnemyState_AttackNear:
		m_modelrender.PlayAnimation(enAnimationClip_AttackNear, 0.1f);
		break;
	case enEnemyState_ReceiveDamage:
		m_modelrender.PlayAnimation(enAnimationClip_ReceiveDamage, 0.1f);
		break;
	case enEnemyState_Down :
		m_modelrender.PlayAnimation(enAnimationClip_Down, 0.1f);
		break;
	}
}

void E_001_enemy::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	(void)clipName;
	//キーの名前が色々
	if (wcscmp(eventName, L"attack_start") == 0){
		m_isUnderAttack = true;
	}
	else if (wcscmp(eventName, L"attack_end") == 0){
		m_isUnderAttack = false;
	}
	else if (wcscmp(eventName, L"attack_point") == 0) {
		SoundSource* se = NewGO<SoundSource>(7);
		se = NewGO<SoundSource>(7);
		se->Init(7);
		se->Play(false);
		auto bullet = NewGO<B_homingbullet>(0);
		bullet->SetPosition(m_position + corre2);
		bullet->SetVelocity(m_forward * 10);
		bullet->SetEnShooter(B_homingbullet::enShooter_Enemy);
		m_effect = NewGO<EffectEmitter>(0);
		m_effect->Init(0);
		m_effect->SetScale({ 10.0f,10.0f,10.0f });
		m_effect->SetPosition(m_position + corre2);
		m_effect->SetRotation(m_rotation);
		m_effect->Play();
	}
}

void E_001_enemy::ProcessCommonStateTransition()
{
	//各タイマーを初期化。
	m_idleTimer = 0.0f;
	m_chaseTimer = 0.0f;
	//エネミーからプレイヤーに向かうベクトルを計算する。
	Vector3 diff = m_player->Getposition() - m_position;
	Vector3 playerPosition = m_player->Getposition();
	//プレイヤーを見つけたら。
	if (SearchPlayer() == true)
	{
		//ベクトルを正規化する。
		diff.Normalize();
		//移動速度を設定する。
		m_movespeed = diff * enemyspeed;
		//攻撃できる距離かどうか
		if (SearchAttackDistance() == true){
			if (m_attackTimer < 0.0f){
				if (SearchAttackDistanceNear() == true){
					m_enemystate = enEnemyState_AttackNear;
				}
				else {
					//プレイヤー位置取得
					m_targetPosition = playerPosition;
					m_enemystate = enEnemyState_Attack;
				}
				m_attackTimer = attacktime;
				return;
			}
			//現在のステートが攻撃
			if (m_enemystate == enEnemyState_Attack||
				m_enemystate == enEnemyState_AttackNear){
				//連続で撃たせないように
				//追跡
				m_enemystate = enEnemyState_Chase;
				return;
			}
			//現在のステートが攻撃でない
			else{
				m_enemystate = enEnemyState_Chase;
				return;
			}
		}
	}
	//プレイヤーを見つけられなければ。
	else
	{
		//待機ステートに遷移する。
		m_attackTimer = attacktime;
		m_enemystate = enEnemyState_Idle;
		return;

	}
}


void E_001_enemy::ProcessIdleStateTransition()
{
	ProcessCommonStateTransition();
}

void E_001_enemy::ProcessChaseStateTransition()
{
	//射程圏内に入ったら
	if (SearchAttackDistance() == true)
	{
		//他のステートに遷移する。
		m_attackTimer -= g_gameTime->GetFrameDeltaTime();
		ProcessCommonStateTransition();
		return;
	}
	ProcessCommonStateTransition();
}

void E_001_enemy::ProcessAttackStateTransition()
{
	//攻撃アニメーションの再生が終わったら。
	if (m_modelrender.IsPlayingAnimation() == false)
	{
		//他のステートに遷移する。
		ProcessCommonStateTransition();
	}
}

void E_001_enemy::ProcessReceiveDamageStateTransition()
{
	//被ダメージアニメーションの再生が終わったら。
	if (m_modelrender.IsPlayingAnimation() == false)
	{
		if (SearchAttackDistanceNear() == true){
			m_enemystate = enEnemyState_AttackNear;
		}
		else{
			m_enemystate = enEnemyState_Attack;
		}
	}
}

void E_001_enemy::ProcessDownStateTransition()
{
	//被ダメージアニメーションの再生が終わったら。
	if (m_modelrender.IsPlayingAnimation() == false)
	{
		SoundSource* se = NewGO<SoundSource>(11);
		se = NewGO<SoundSource>(11);
		se->Init(11);
		se->Play(false);
		ItemDrop();
		DeleteGoEnemyList();
		DeleteGO(m_collisionObject);//消去処理
		DeleteGO(this);
	}
}

void E_001_enemy::Render(RenderContext& rc)
{
	m_modelrender.Draw(rc);
	//if (m_enemysheld_type == SheldEnemy) {
	//	if (m_effectSH->IsPlay() == false) {
	//		Shplay();
	//	}
	//}
}