#include "stdafx.h"
#include "I_G_Item.h"

namespace {
	Vector3 corre = { 0.0f,20.0f,0.0f };
}

bool I_G_Item::Start()
{
	//エフェクト読み込み

	m_effect = NewGO<EffectEmitter>(2);
	m_effect->Init(2);
	m_effect->SetScale({ 10.0f,10.0f,10.0f });
	//座標
	m_effect->SetPosition(m_position + corre);
	m_effect->Play();
	m_looptimer = looptime;
	//コリジョンオブジェクトを作成する。
	m_collisionObject = NewGO<CollisionObject>(0);
	//球状のコリジョンを作成する。
	m_collisionObject->CreateSphere(m_position, Quaternion::Identity, 30.0f * m_scale.z);
	m_collisionObject->SetName("Item_col_G");
	m_collisionObject->SetPosition(m_position + corre);
	//コリジョンオブジェクトが自動で削除されないようにする。
	m_collisionObject->SetIsEnableAutoDelete(false);

	m_player = FindGO<P_main_Player>("player");
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);

	m_autodeletetimer = autodeletetime;
	return true;
}

void I_G_Item::Update()
{
	//当たり判定処理
	Collision();
	//アイテム消去処理
	deleteitem();
	deleteitemAuto();
	//描画処理
	EffectDraw();
}

//描画処理
void I_G_Item::EffectDraw()
{
	if (m_looptimer <= 0) {
		m_effect->Play();
		m_looptimer = looptime;
	}
	else {
		m_looptimer -= 0.01;
	}
}

