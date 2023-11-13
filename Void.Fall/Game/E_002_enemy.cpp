#include "stdafx.h"
#include "E_002_enemy.h"
#include "IEnemy.h"
#include "Game.h"
#include "collision/CollisionObject.h"
#include "P_main_Player.h"
#define enemyspeed 100.0f                               //移動速度の数値
#define enemyserch 700.0f * 700.0f						//追跡可能範囲

namespace
{
	const Vector3 corre = { 0.0f,60.0f,0.0f };//位置修正
}

bool E_002_enemy::Start()
{

	//アニメーション読み込み
	m_animationclips[enAnimationClip_Idle].Load("Assets/animData/Enemy/enemy_002/idle.tka");
	m_animationclips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationclips[enAnimationClip_Chase].Load("Assets/animData/Enemy/enemy_002/walk.tka");
	m_animationclips[enAnimationClip_Chase].SetLoopFlag(true);

	//モデル読み込み
	m_modelrender = new ModelRender;
	m_modelrender->Init("Assets/modelData/Enemy/enemy_002/RE_enemy_002.tkm", m_animationclips, enAnimationClip_Num);

	//回転
	m_modelrender->SetRotation(m_rotation);
	//座標
	m_modelrender->SetPosition(m_position);
	//スケール
	m_modelrender->SetScale(m_scale);
	//キャラコン初期化
	m_charaCon.Init(20.0f, 100.0f, m_position);

	//コリジョンオブジェクトを作成する。
	m_collisionObject = NewGO<CollisionObject>(0);
	//球状のコリジョンを作成する。
	m_collisionObject->CreateSphere(m_position, Quaternion::Identity, 60.0f * m_scale.z);
	m_collisionObject->SetName("enemy_col");
	m_collisionObject->SetPosition(m_position + corre);
	//コリジョンオブジェクトが自動で削除されないようにする。
	m_collisionObject->SetIsEnableAutoDelete(false);

	m_player = FindGO<P_main_Player>("player");
	SetenemyList();

	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
	return true;
}

void E_002_enemy::Update()
{
	//追跡処理
	Chase();
	//回転処理
	Rotation();
	//アニメーション
	PlayAnimation();
	//ステート遷移処理
	ManageState();
	//描画更新
	m_modelrender->Update();
}

void E_002_enemy::PlayAnimation()
{
	m_modelrender->SetAnimationSpeed(1.0f);
	switch (m_enemystate)
	{
		//待機
	case enEnemyState_Idle:
		m_modelrender->PlayAnimation(enAnimationClip_Idle, 0.1f);
		break;
	case enEnemyState_Chase:
		m_modelrender->PlayAnimation(enAnimationClip_Chase, 0.1f);
		break;
	}
}

void E_002_enemy::ManageState()
{
	switch (m_enemystate)
	{
	case E_002_enemy::enEnemyState_Idle:
		ProcessIdleStateTransition();
		break;
	case E_002_enemy::enEnemyState_Chase:
		ProcessChaseStateTransition();
		break;
	}
}

void E_002_enemy::ProcessCommonStateTransition()
{
	Vector3 diff = m_player->Getposition() - m_position;

	if (SearchPlayer() == true)
	{
		//ベクトルを正規化する。
		diff.Normalize();
		//移動速度を設定する。
		m_movespeed = diff * enemyspeed;
		m_enemystate = enEnemyState_Chase;
	}
	else
	{
		m_enemystate = enEnemyState_Idle;
	}
}

void E_002_enemy::ProcessIdleStateTransition()
{
	ProcessCommonStateTransition();
}

void E_002_enemy::ProcessChaseStateTransition()
{
	ProcessCommonStateTransition();
}

void E_002_enemy::Chase()
{
	//追跡ステートでないなら、追跡処理はしない。
	if (m_enemystate != enEnemyState_Chase)
	{
		return;
	}

	//エネミーを移動させる。
	m_position = m_charaCon.Execute(m_movespeed, g_gameTime->GetFrameDeltaTime());
	if (m_charaCon.IsOnGround()) {
		//地面についた。
		//重力を0にする。
		m_movespeed.y = 0.0f;
	}
	Vector3 modelPosition = m_position;
	//ちょっとだけモデルの座標を挙げる。
	modelPosition.y += 2.5f;
	//座標を設定する。
	m_modelrender->SetPosition(modelPosition);
}

void E_002_enemy::Rotation()
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
	m_modelrender->SetRotation(m_rotation);
	m_collisionObject->SetRotation(m_rotation);
	m_collisionObject->SetPosition(m_position + corre);

	//プレイヤーの前ベクトルを計算する。
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
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

void E_002_enemy::Render(RenderContext& rc)
{
	m_modelrender->Draw(rc);
}


