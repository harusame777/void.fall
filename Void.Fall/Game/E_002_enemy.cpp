#include "stdafx.h"
#include "E_002_enemy.h"
#include "IEnemy.h"
#include "Game.h"
#include "collision/CollisionObject.h"
#include "P_main_Player.h"
#define enemyspeed 150.0f                               //移動速度の数値
#define enemyattackspeed 300.0f                         //攻撃時移動速度の数値
#define enemyserch 700.0f * 700.0f						//追跡可能範囲

namespace
{
	const Vector3 corre1 = { 0.0f,60.0f,0.0f };//位置修正
	const Vector3 corre2 = { 2.0f,2.0f,2.0f };
}

bool E_002_enemy::Start()
{

	//アニメーション読み込み
	m_animationclips[enAnimationClip_Idle].Load("Assets/animData/Enemy/enemy_002/idle.tka");
	m_animationclips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationclips[enAnimationClip_Chase].Load("Assets/animData/Enemy/enemy_002/walk.tka");
	m_animationclips[enAnimationClip_Chase].SetLoopFlag(true);
	m_animationclips[enAnimationClip_Attack].Load("Assets/animData/Enemy/enemy_002/attack.tka");
	m_animationclips[enAnimationClip_Attack].SetLoopFlag(true);
	m_animationclips[enAnimationClip_ReceiveDamage].Load("Assets/animData/Enemy/enemy_002/receivedamage.tka");
	m_animationclips[enAnimationClip_ReceiveDamage].SetLoopFlag(false);
	m_animationclips[enAnimationClip_Down].Load("Assets/animData/Enemy/enemy_002/down.tka");
	m_animationclips[enAnimationClip_Down].SetLoopFlag(false);


	//モデル読み込み
	m_modelrender.Init("Assets/modelData/Enemy/enemy_002/RE_enemy_002.tkm", m_animationclips, enAnimationClip_Num);

	//回転
	m_modelrender.SetRotation(m_rotation);
	//座標
	m_modelrender.SetPosition(m_position);
	//スケール
	m_modelrender.SetScale(m_scale);
	//キャラコン初期化
	m_charaCon.Init(20.0f, 100.0f, m_position);

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

void E_002_enemy::Update()
{
	//追跡処理
	Chase();
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

void E_002_enemy::PlayAnimation()
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
	case enEnemyState_ReceiveDamage:
		m_modelrender.PlayAnimation(enAnimationClip_ReceiveDamage, 0.1f);
		break;
	case enEnemyState_Down:
		m_modelrender.PlayAnimation(enAnimationClip_ReceiveDamage, 0.1f);
		break;

	}
}

void E_002_enemy::ManageState()
{
	switch (m_enemystate)
	{
	case enEnemyState_Idle:
		ProcessIdleStateTransition();
		break;
	case enEnemyState_Chase:
		ProcessChaseStateTransition();
		break;
	case enEnemyState_Attack:
		ProcessAttackStateTransition();
		break;
	case enEnemyState_ReceiveDamage:
		ProcessReceiveDamageStateTransition();
		break;
	case enEnemyState_Down:
		ProcessDownStateTransition();
		break;
	}
}

void E_002_enemy::ProcessCommonStateTransition()
{
	if (m_attackcooltimer > 0){
		m_attackcooltimer -= g_gameTime->GetFrameDeltaTime();
		return;
	}
	if (m_enemystate == enEnemyState_Attack){
		return;
	}
	Vector3 diff = m_player->Getposition() - m_position;

	//ベクトルを正規化する。
	diff.Normalize();
	//移動速度を設定する。
	m_movespeed = diff * enemyspeed;
	if (SearchPlayer())
	{
		if (SearchAttackDistance()){
			if (m_attackTimer < 0.0f) {
				m_enemystate = enEnemyState_Attack;
				m_attackTimer = attacktime;
				m_attackGotimer = attackGotime;
				m_movespeed = diff * enemyattackspeed;
				return;
			}
			if (m_enemystate == enEnemyState_Attack) {
				m_enemystate = enEnemyState_Chase;
				return;
			}
			else {
				m_enemystate = enEnemyState_Chase;
				return;
			}
		}
	}
	else
	{
		m_attackTimer = attacktime;
		m_enemystate = enEnemyState_Idle;
		return;
	}
}

void E_002_enemy::ProcessIdleStateTransition()
{
	ProcessCommonStateTransition();
}

void E_002_enemy::ProcessChaseStateTransition()
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

void E_002_enemy::ProcessAttackStateTransition()
{
	if (m_attackGotimer >= 0){
		ProcessCommonStateTransition();
	}
	else
	{
		m_enemystate = enEnemyState_Idle;
		DeleteGO(m_attackcoll);
		m_attackcooltimer = attackcooltime;
		ProcessCommonStateTransition();
	}
}

void E_002_enemy::ProcessReceiveDamageStateTransition()
{
	//被ダメージアニメーションの再生が終わったら。
	if (m_modelrender.IsPlayingAnimation() == false)
	{
		ProcessCommonStateTransition();
	}
}

void E_002_enemy::ProcessDownStateTransition()
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

void E_002_enemy::Chase()
{
	//追跡ステートでないなら、追跡処理はしない。
	if (IsAttack()){
		return;
	}
	if (m_attackGotimer > 0){
		m_attackGotimer -= g_gameTime->GetFrameDeltaTime();
	}
	//エネミーを移動させる。
	m_position = m_charaCon.Execute(m_movespeed, g_gameTime->GetFrameDeltaTime());
	if (m_charaCon.IsOnGround()) {
		//地面についた。
		//重力を0にする。
		m_movespeed.y = 0.0f;
	}
	Vector3 modelPosition = m_position;
	m_collisionObject->SetRotation(m_rotation);
	m_collisionObject->SetPosition(m_position + corre1);
	//ちょっとだけモデルの座標を挙げる。
	modelPosition.y += 2.5f;
	//座標を設定する。
	m_modelrender.SetPosition(modelPosition);
}

void E_002_enemy::Rotation()
{
	if (m_enemystate == enEnemyState_Attack){
		return;
	}
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

	//プレイヤーの前ベクトルを計算する。
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}

void E_002_enemy::Attack()
{
	if (m_enemystate == enEnemyState_Attack){
		//攻撃当たり判定用のコリジョンオブジェクトを作成する。
		m_attackcoll = NewGO<CollisionObject>(0);
		//球状のコリジョンを作成する。
		m_attackcoll->CreateSphere(m_position, Quaternion::Identity, 50.0f * m_scale.z);
		m_attackcoll->SetName("enemy_attack");
		Vector3 position = m_position + m_forward * 40.0f;
		m_attackcoll->SetPosition(position + corre1);
	}
}

void E_002_enemy::Collision()
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
			if (m_sh > 0) {
				m_sh--;
				//被ダメージステートに遷移する。
				m_enemystate = enEnemyState_ReceiveDamage;
				return;
			}
			else {
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


const bool E_002_enemy::SearchPlayer() const
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

const bool E_002_enemy::SearchAttackDistance() const
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

void E_002_enemy::Render(RenderContext& rc)
{
	m_modelrender.Draw(rc);
	if (m_enemysheld_type == SheldEnemy) {
		if (m_effectSH->IsPlay() == false) {
			Shplay();
		}
	}
}


