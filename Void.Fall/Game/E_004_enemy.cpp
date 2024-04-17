#include "stdafx.h"
#include "E_004_enemy.h"
#include "M_parts4.h"
#include "math.h"
#include "E_001_enemy.h"
#include "E_002_enemy.h"
#include "IEnemy.h"
#include "M_parts5.h"

namespace
{
	const Vector3 corre1 = { 0.0f,60.0f,0.0f };//位置修正
	const Vector3 SummonCorreA = { 200.0f,-40.0f,0.0f }; //召喚位置修正A
	const Vector3 SummonCorreB = { -200.0f,-40.0f,0.0f }; //召喚位置修正B
	const Vector3 SummonCorreC = { 0.0f,-40.0f,200.0f }; //召喚位置修正C
	const Vector3 SummonCorreD = { 0.0f,-40.0f,-200.0f }; //召喚位置修正D
 }

bool E_004_enemy::Start()
{
	m_animationclips[enAnimationClip_Standby].Load("Assets/animData/Enemy/enemy_004/standby.tka");
	m_animationclips[enAnimationClip_Standby].SetLoopFlag(true);
	m_animationclips[enAnimationClip_Active].Load("Assets/animData/Enemy/enemy_004/active.tka");
	m_animationclips[enAnimationClip_Active].SetLoopFlag(true);
	m_animationclips[enAnimationClip_Down].Load("Assets/animData/Enemy/enemy_004/down.tka");
	m_animationclips[enAnimationClip_Down].SetLoopFlag(false);

	m_modelrender.Init("Assets/modelData/Enemy/enemy_004/enemy_004.tkm"
	, m_animationclips, enAnimationClip_Num);
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
	m_parts5 = FindGO<M_parts5>("parts5");

	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);

	EnemyGoEffect();

	return true;
}

void E_004_enemy::Update()
{
	//当たり判定処理
	Collision();
	//アニメーション
	PlayAnimation();
	//遷移処理
	ManageState();
	//描画処理
	m_modelrender.Update();
}

void E_004_enemy::ManageState()
{
	switch (m_enemystate)
	{
	case E_004_enemy::enEnemyState_Standby:
		return;
		break;
	case E_004_enemy::enEnemyState_Active:
		//起動状態時の処理を実行する。
		ActiveLock();
		break;
	case E_004_enemy::enEnemyState_Down:
		ProcessDownStateTransition();
		break;
	}
}

void E_004_enemy::PlayAnimation()
{
	m_modelrender.SetAnimationSpeed(1.0f);
	switch (m_enemystate)
	{
	case E_004_enemy::enEnemyState_Standby:
		m_modelrender.PlayAnimation(enAnimationClip_Standby, 0.1f);
		break;
	case E_004_enemy::enEnemyState_Active:
		m_modelrender.PlayAnimation(enAnimationClip_Active, 0.1f);
		break;
	case E_004_enemy::enEnemyState_Down:
		m_modelrender.PlayAnimation(enAnimationClip_Down, 0.1f);
		break;
	}
}

void E_004_enemy::Collision()
{
	if (m_player != nullptr) {
		Vector3 diff = m_player->m_position - m_position;
		if (diff.Length() <= 300.0f && m_enemystate == enEnemyState_Standby) {
			m_enemystate = enEnemyState_Active;
			m_game->m_Map4List[map_num]->mapLockOn();
		}
	}
}

void E_004_enemy::ActiveLock()
{
	if (EnemyOneSum == false){
		SoundSource* se = NewGO<SoundSource>(9);
		se = NewGO<SoundSource>(9);
		se->Init(9);
		se->Play(false);
		EnemySummon();
		EnemyOneSum = true;
	}
	if (m_game->SummonEnemynum == 0){
		m_parts5->GoalLockNum--;
		m_enemystate = enEnemyState_Down;
	}
}

void E_004_enemy::EnemySummon()
{
	for (int i = 0; i < 4; i++){
		//int enemyrand = Random();
		EnemyRand(i);
	}
}

void E_004_enemy::EnemyRand(/*int randnum,*/int Vecnum)
{
	switch (Vecnum)
	{
	case 0:
	case 1:
	{E_001_enemy* enemy_001 = NewGO<E_001_enemy>(0, "summonenemy");
	enemy_001->Setposition(m_position + EnemySetVec(Vecnum));
	enemy_001->Setrotation(m_rotation);
	enemy_001->SetHP(3);
	//enemy_001->SheldRand();
	enemy_001->SetVectornum(m_game->m_numenemy);
	enemy_001->SetEnemyType(E_001_enemy::en_enemy001);
	enemy_001->SetSummonType(E_001_enemy::Enemy4Sum);
	m_game->m_numenemy++;
	m_game->SummonEnemynum++;
	m_game->m_EnemyList.push_back(enemy_001); 		
	break;}
	case 2:
	case 3:
	{E_002_enemy* enemy_002 = NewGO<E_002_enemy>(0, "summonenemy");
	enemy_002->Setposition(m_position + EnemySetVec(Vecnum));
	enemy_002->Setrotation(m_rotation);
	enemy_002->SetHP(3);
	//enemy_002->SheldRand();
	enemy_002->SetVectornum(m_game->m_numenemy);
	enemy_002->SetEnemyType(E_002_enemy::en_enemy002);
	enemy_002->SetSummonType(E_002_enemy::Enemy4Sum);
	m_game->m_numenemy++;
	m_game->SummonEnemynum++;
	m_game->m_EnemyList.push_back(enemy_002); 		
	break;}
	}
}

Vector3 E_004_enemy::EnemySetVec(int Vecnum)
{
	switch (Vecnum)
	{
	case 0:
		return SummonCorreA;
		break;
	case 1:
		return SummonCorreB;
		break;
	case 2:
		return SummonCorreC;
		break;
	case 3:
		return SummonCorreD;
		break;
	}
}

void E_004_enemy::ProcessDownStateTransition()
{
	//被ダメージアニメーションの再生が終わったら。
	if (m_modelrender.IsPlayingAnimation() == false)
	{
		SoundSource* se = NewGO<SoundSource>(11);
		se = NewGO<SoundSource>(11);
		se->Init(11);
		se->Play(false);
		ItemDrop();
		DeleteGO(m_collisionObject);//消去処理
		DeleteGO(this);
	}
}

void E_004_enemy::Render(RenderContext& rc)
{
	m_modelrender.Draw(rc);
}


