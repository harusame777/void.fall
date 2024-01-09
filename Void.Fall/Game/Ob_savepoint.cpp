#include "stdafx.h"
#include "Ob_savepoint.h"
#include "P_main_Player.h"
#include "Game.h"

namespace
{
	const Vector3 corre1 = { 0.0f,60.0f,0.0f };//位置修正
}

bool Ob_savepoint::Start()
{
	m_animationclips[enAnimationClip_Standby].Load("Assets/modelData/A_object/savepoint/normal.tka");
	m_animationclips[enAnimationClip_Standby].SetLoopFlag(true);
	m_animationclips[enAnimationClip_Save].Load("Assets/modelData/A_object/savepoint/save.tka");
	m_animationclips[enAnimationClip_Save].SetLoopFlag(false);

	//モデル読み込み
	m_modelrender.Init("Assets/modelData/A_object/savepoint/savepoint.tkm"
		, m_animationclips, enAnimationClip_Num);
	//座標設定
	m_modelrender.SetPosition(m_position);
	//回転値設定
	m_modelrender.SetRotation(m_rotation);
	//大きさ設定
	m_modelrender.SetScale(m_scale);
	m_modelrender.Update();

	//当たり判定作成
	m_physicsStaticObject.CreateFromModel(m_modelrender.GetModel(), 
	m_modelrender.GetModel().GetWorldMatrix());
	//コリジョンオブジェクトを作成する。
	m_collisionObject = NewGO<CollisionObject>(0);
	//球状のコリジョンを作成する。
	m_collisionObject->CreateSphere(m_position, Quaternion::Identity, 60.0f * m_scale.z);
	m_collisionObject->SetName("enemy_col");
	m_collisionObject->SetPosition(m_position + corre1);
	//コリジョンオブジェクトが自動で削除されないようにする。
	m_collisionObject->SetIsEnableAutoDelete(false);

	m_game = FindGO<Game>("game");
	m_player = FindGO<P_main_Player>("player");

	SetsaveVec();
	return true;
}

void Ob_savepoint::Update()
{
	//当たり判定処理
	Collision();
	//アニメーション処理
	PlayAnimation();
	//遷移処理
	ManageState();
	//描画処理
	m_modelrender.Update();
}

void Ob_savepoint::Collision()
{
	//プレイヤーの攻撃用のコリジョンを取得する。
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_attack");
	//コリジョンの配列をfor文で回す。
	for (auto collision : collisions)
	{
		//コリジョンとキャラコンが衝突したら。
		if (collision->IsHit(m_collisionObject))
		{
			m_state = en_save;
		}
	}
}

void Ob_savepoint::PlayAnimation()
{
	switch (m_state)
	{
	case Ob_savepoint::en_Standby:
		m_modelrender.PlayAnimation(enAnimationClip_Standby, 0.1f);
		break;
	case Ob_savepoint::en_save:
		m_modelrender.PlayAnimation(enAnimationClip_Save, 0.1f);
		break;
	}
}

void Ob_savepoint::ManageState()
{
	switch (m_state)
	{
	case Ob_savepoint::en_Standby:
		return;
		break;
	case Ob_savepoint::en_save:
		ProcessSaveStateTransition();
		break;
	}
}

void Ob_savepoint::ProcessSaveStateTransition()//セーブ遷移
{
	//アニメーションの再生が終わったら。
	if (m_modelrender.IsPlayingAnimation() == false)
	{
		m_game->m_Nowsavepointnum = savenum;
		m_state = en_Standby;
	}
}

void Ob_savepoint::Render(RenderContext& rc){
	//描画処理
	m_modelrender.Draw(rc);
}

