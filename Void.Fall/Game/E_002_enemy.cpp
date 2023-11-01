#include "stdafx.h"
#include "E_002_enemy.h"
#include "IEnemy.h"
#include "Game.h"
bool E_002_enemy::Start()
{

	//アニメーション読み込み
	m_animationclips[enAnimationClip_Idle].Load("Assets/animData/Enemy/enemy_002/idle.tka");
	m_animationclips[enAnimationClip_Idle].SetLoopFlag(true);

	//モデル読み込み
	m_modelrender = new ModelRender;
	m_modelrender->Init("Assets/modelData/Enemy/enemy_002/enemy_002.tkm", m_animationclips, enAnimationClip_Num);

	//回転
	m_modelrender->SetRotation(m_rotation);
	//座標
	m_modelrender->SetPosition(m_position);
	//スケール
	m_modelrender->SetScale(m_scale);
	//キャラコン初期化
	m_charaCon.Init(20.0f, 100.0f, m_position);

	return true;
}

void E_002_enemy::Render(RenderContext& rc)
{
	m_modelrender->Draw(rc);
}

void E_002_enemy::Update()
{
	m_modelrender->Update();
}

