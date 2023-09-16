#include "stdafx.h"
#include "Map_home1.h"

bool Map_home1::Start()
{
	//モデル読み込み
	m_modelrender.Init("Assets/modelData/A_testmap/testmap.tkm");
	//座標設定
	m_modelrender.SetPosition(m_position);
	//回転値設定
	m_modelrender.SetRotation(m_rotation);
	//大きさ設定
	m_modelrender.SetScale(m_scale);
	m_modelrender.Update();
	//当たり判定作成
	m_physicsStaticObject.CreateFromModel(m_modelrender.GetModel(), m_modelrender.GetModel().GetWorldMatrix());
	//当たり判定を可視化する。
	PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
	return true;
}

void Map_home1::Update()
{
	//描画更新
	m_modelrender.Update();
}

void Map_home1::Render(RenderContext& rc)
{
	//描画処理
	m_modelrender.Draw(rc);
}