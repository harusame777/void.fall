#include "stdafx.h"
#include "M_parts1_move.h"
#define Move1 0
#define Move2 1

bool M_parts1_move::Start()
{
	//モデル読み込み
	m_modelrender.Init("Assets/modelData/A_mapParts/stage1/mapparts1.tkm");
	//座標設定
	m_modelrender.SetPosition(m_position);
	//回転値設定
	m_modelrender.SetRotation(m_rotation);
	//大きさ設定
	m_modelrender.SetScale(m_scale);
	m_modelrender.Update();

	SetmoveVec();
	m_physicsStaticObject.CreateFromModel(m_modelrender.GetModel(), m_modelrender.GetModel().GetWorldMatrix());
	return true;
}

void M_parts1_move::Update()
{
	//移動処理
	Move();
	// 当たり判定の座標を更新
	m_physicsStaticObject.GetRigitBody()->SetPositionAndRotation(m_position, m_rotation);
	m_modelrender.Update();
}

void M_parts1_move::Move()
{
	Vector3 diff = m_moveVec[MoveSwitch] - m_position;
	float Dis = diff.Length();
	if (Dis < 20.0f){
		if (MoveSwitch == 0) MoveSwitch = 1;
		else MoveSwitch = 0;
	}
	Vector3 toPosDis = diff;
	toPosDis.Normalize();
	m_position += toPosDis * 2.0f;
	m_modelrender.SetPosition(m_position);
}

void M_parts1_move::Render(RenderContext& rc) {
	//描画処理
	m_modelrender.Draw(rc);
}