#include "stdafx.h"
#include "M_parts5.h"
#include "Game.h"
#include "P_main_Player.h"
#include "GameClear.h"

namespace
{
	const Vector3 corre1 = { 0.0f,60.0f,0.0f };//位置修正
}

bool M_parts5::Start()
{
	//モデル読み込み
	m_modelrender.Init("Assets/modelData/A_mapParts/stage1/mapparts5.tkm");
	//座標設定
	m_modelrender.SetPosition(m_position);
	//回転値設定
	m_modelrender.SetRotation(m_rotation);
	//大きさ設定
	m_modelrender.SetScale(m_scale);
	m_modelrender.Update();

	//コリジョンオブジェクトを作成する。
	m_collisionObject = NewGO<CollisionObject>(0);
	//球状のコリジョンを作成する。
	m_collisionObject->CreateSphere(m_position, Quaternion::Identity, 60.0f * m_scale.z);
	m_collisionObject->SetName("goal_col");
	m_collisionObject->SetPosition(m_position + corre1);
	//コリジョンオブジェクトが自動で削除されないようにする。
	m_collisionObject->SetIsEnableAutoDelete(false);

	m_player = FindGO<P_main_Player>("player");
	m_game = FindGO<Game>("game");

	SetPhysics();
	return true;
}

void M_parts5::Update()
{
	//ゴール判定出現
	if (GoalLockNum < 0){
		Collision();
	}
	//描画更新
	m_modelrender.Update();
}

void M_parts5::Lock()
{
}

void M_parts5::Collision()
{
	if (Deletemap == true){
		if (m_collisionObject->IsHit(m_player->m_charaCon)){
			Deletemap = false;
			NewGO <GameClear>(0, "gameclear");
		}
	}
}

void M_parts5::Render(RenderContext& rc) 
{
	//描画処理
	m_modelrender.Draw(rc);
}