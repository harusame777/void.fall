#include "stdafx.h"
#include "E_001_enemy.h"
#include "P_main_Player.h"
///////////////////////////////////////////////////////////
#include <time.h>
#include <stdlib.h>
///////////////////////////////////////////////////////////
#define enemyspeed 100.0f

bool E_001_enemy::Start()
{
	m_modelrender = new ModelRender;
	m_modelrender->Init("Assets/modelData/Enemy/enemy_001/enemy_001.tkm");
	//回転
	m_modelrender->SetRotation(m_rotation);
	//座標
	m_modelrender->SetPosition(m_position);
	//スケール
	m_modelrender->SetScale(m_scale);
	//キャラコン初期化
	m_charaCon.Init(
		20.0f,			//半径。
		100.0f,			//高さ。
		m_position		//座標。
	);
	m_player = FindGO<P_main_Player>("player");
	//乱数を初期化。
	srand((unsigned)time(NULL));
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
	return true;
}

void E_001_enemy::Update()
{
	//追跡処理
	Chase();
	//回転処理
	Rotation();
	//攻撃処理
	Attack();
	//ステート遷移処理
	ManageState();
	//描画更新
	m_modelrender->Update();
}

void E_001_enemy::Chase()
{
	//追跡ステートでないなら、追跡処理はしない。
	if (m_enemystate != enEnemyState_Chase)
	{
		return;
	}
	//プレイヤーが射程圏内に入ったら攻撃処理
	if (SearchAttackDistance() == true)
	{
		m_enemystate = enEnemyState_Attack;
		return;
	}
	//エネミーを移動させる。
	m_position = m_charaCon.Execute(m_movespeed, g_gameTime->GetFrameDeltaTime());
	//if (m_charaCon.IsOnGround()) {
	//	//地面についた。
	//	//重力を0にする。
	//	m_movespeed.y = 0.0f;
	//}
	m_movespeed.y = 0.0f;
	Vector3 modelPosition = m_position;
	//ちょっとだけモデルの座標を挙げる。
	modelPosition.y += 2.5f;
	//座標を設定する。
	m_modelrender->SetPosition(modelPosition);
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
	m_modelrender->SetRotation(m_rotation);

	//プレイヤーの前ベクトルを計算する。
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}

const bool E_001_enemy::SearchPlayer() const
{
	Vector3 diff = m_player->Getposition() - m_position;

	//プレイヤーにある程度近かったら.。
	if (diff.LengthSq() <= 700.0 * 700.0f)
	{
		//エネミーからプレイヤーに向かうベクトルを正規化する。
		diff.Normalize();
		//エネミーの正面のベクトルと、エネミーからプレイヤーに向かうベクトルの。
		//内積(cosθ)を求める。
		float cos = m_forward.Dot(diff);
		//内積(cosθ)から角度(θ)を求める。
		float angle = acosf(cos);
		//角度(θ)が120°より小さければ。
		//if (angle <= (Math::PI / 180.0f) * 140.0f)
		//{
			//プレイヤーを見つけた！
			return true;
		//}
	}
	//プレイヤーを見つけられなかった。
	return false;
}

const bool E_001_enemy::SearchAttackDistance() const
{
	Vector3 diff = m_player->Getposition() - m_position;
	//プレイヤーにある程度近かったら.。

	if (diff.LengthSq() <= 200.0 * 200.0f)
	{
		//プレイヤーが射程圏内に入った！
		return true;
	}
	//プレイヤーが射程圏外だった。
	return false;
}

void E_001_enemy::Attack()
{
	if (m_enemystate != enEnemyState_Attack)
	{
		return;
	}
	
	if (m_isUnderAttack == true)
	{

	}
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
		//攻撃ステート遷移

	}
}

void E_001_enemy::ProcessCommonStateTransition()
{
	//各タイマーを初期化。
	m_idleTimer = 0.0f;
	m_chaseTimer = 0.0f;

	//エネミーからプレイヤーに向かうベクトルを計算する。
	Vector3 diff = m_player->Getposition() - m_position;
	//プレイヤーを見つけたら。
	if (SearchPlayer() == true)
	{
		//ベクトルを正規化する。
		diff.Normalize();
		//移動速度を設定する。
		m_movespeed = diff * enemyspeed;
		//攻撃できる距離かどうか
		if (SearchAttackDistance() == true)
		{
			//乱数によって攻撃するかどうかを決める
			int ram = rand() % 100;
			if (ram > 30)
			{
				//追跡
				m_enemystate = enEnemyState_Chase;
			}
			else
			{
				//現在のステートが攻撃
				if (m_enemystate == enEnemyState_Attack)
				{
					//連続で撃たせないように
					//追跡
					m_enemystate = enEnemyState_Chase;
					return;
				}
				//現在のステートが攻撃でない
				else
				{
					m_enemystate = enEnemyState_Attack;
					return;
				}
			}
		}
	}
	//プレイヤーを見つけられなければ。
	else
	{
		//待機ステートに遷移する。
		m_enemystate = enEnemyState_Idle;
		return;

	}
}


void E_001_enemy::ProcessIdleStateTransition()
{
	m_idleTimer += g_gameTime->GetFrameDeltaTime();
	//待機時間がある程度経過したら。
	if (m_idleTimer >= 0.9f)
	{
		//他のステートへ遷移する。
		ProcessCommonStateTransition();
	}
}

void E_001_enemy::ProcessChaseStateTransition()
{
	//射程圏内に入ったら
	if (SearchAttackDistance() == true)
	{
	//他のステートに遷移する。
	ProcessCommonStateTransition();
	return;
	}
	m_chaseTimer += g_gameTime->GetFrameDeltaTime();
	//追跡時間がある程度経過したら。
	if (m_chaseTimer >= 0.8f)
	{
		//他のステートに遷移する。
		ProcessCommonStateTransition();
	}
}

void E_001_enemy::Render(RenderContext& rc)
{
	m_modelrender->Draw(rc);
}