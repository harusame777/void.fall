#include "stdafx.h"
#include "E_004_enemy.h"
#include "M_parts4.h"
#include "math.h"

namespace
{
	const Vector3 corre1 = { 0.0f,60.0f,0.0f };//位置修正
}

bool E_004_enemy::Start()
{
	//m_animationclips[enAnimationClip_Standby].Load("Assets/animData/Enemy/enemy_004/standby.tka");
	//m_animationclips[enAnimationClip_Standby].SetLoopFlag(true);


	m_modelrender.Init("Assets/modelData/Enemy/enemy_004/enemy_004.tkm"
	/*, m_animationclips, enAnimationClip_Num*/);
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
	m_parts4 = FindGO<M_parts4>("parts4");

	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);

	EnemyGoEffect();

	return true;
}

void E_004_enemy::Update()
{
	//もしステートが起動状態だったら。
	if (m_enemystate == enEnemyState_Active){
		//起動状態時の処理を実行する。
		ActiveLock();
	}
	//当たり判定処理
	Collision();
	//アニメーション
	PlayAnimation();
	//描画処理
	m_modelrender.Update();
}

void E_004_enemy::PlayAnimation()
{
	//m_modelrender.SetAnimationSpeed(1.0f);
	//switch (m_enemystate)
	//{
	//case E_004_enemy::enEnemyState_Standby:
	//	m_modelrender.PlayAnimation(enAnimationClip_Standby, 0.1f);
	//	break;
	//}
}

void E_004_enemy::Collision()
{
	//プレイヤーの攻撃用のコリジョンを取得する。
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_attack");
	//コリジョンの配列をfor文で回す。
	for (auto collision : collisions)
	{
		//コリジョンとキャラコンが衝突したら。
		if (collision->IsHit(m_collisionObject))
		{
			if (m_enemystate == enEnemyState_Standby){
				m_enemystate = enEnemyState_Active;
				m_Locktimer = Locktime;
				m_parts4->mapLockOn();
				EnemySummon();
			}
		}
	}
}

void E_004_enemy::ActiveLock()
{
	m_Locktimer -= g_gameTime->GetFrameDeltaTime();
	float timer0 = fmod(m_Locktimer,20.0f);
	if (timer0 == 0){
		EnemySummon();
	}
	if (m_Locktimer > 0){

	}
}

void E_004_enemy::EnemySummon()
{

}

void E_004_enemy::Render(RenderContext& rc)
{
	m_modelrender.Draw(rc);
}


