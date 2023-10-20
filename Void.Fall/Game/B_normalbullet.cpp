#include "stdafx.h"
#include "B_normalbullet.h"
#include "collision/CollisionObject.h"
#include "P_main_Player.h"
namespace
{
	const Vector3 scale = { 1.5f,1.5f,1.5f }; //大きさ
	const Vector3 corre = { 0.0f,60.0f,0.0f };//位置修正
}

bool B_normalbullet::Start()
{

	//アニメーション読み込み
	m_animationclips[enAnimationClip_Move].Load
	("Assets/modelData/A_attack/bullet/normalbullet/normalbullet.tka");
	m_animationclips[enAnimationClip_Move].SetLoopFlag(true);

	//モデル読み込み
	m_modelrender = new ModelRender;
	m_modelrender->Init("Assets/modelData/A_attack/bullet/normalbullet/normalbullet1.tkm",
		m_animationclips, enAnimationClip_Num);

	m_modelrender->SetPosition(m_position);
	m_modelrender->SetScale(scale);

	

	//移動速度を計算。
	m_velocity = Vector3::AxisZ;
	m_rotation.Apply(m_velocity);
	m_position += m_velocity * 50.0f;
	m_velocity *= 1200.0f;
	m_rotation.AddRotationDegY(360.0f);

	//コリジョンオブジェクトを作成する。
	m_collisionObject = NewGO<CollisionObject>(0);
	//球状のコリジョンを作成する。
	m_collisionObject->CreateSphere(m_position, Quaternion::Identity, 20.0f * m_scale.z);
	m_collisionObject->SetName("player_attack");
	//コリジョンオブジェクトが自動で削除されないようにする。
	m_collisionObject->SetIsEnableAutoDelete(false);

	return true;
}

void B_normalbullet::Update()
{
	//弾丸移動
	Movebullet();
	//回転処理
	Rotation();
	//弾丸時間消滅処理
	Inpacttime();
	//アニメーション
	PlayAnimation();
	//描画処理
	m_modelrender->Update();
}

void B_normalbullet::Movebullet()
{
	//座標を移動させる。
	m_position += m_velocity *g_gameTime->GetFrameDeltaTime();
	m_modelrender->SetPosition(m_position);
	m_collisionObject->SetPosition(m_position);
}

void B_normalbullet::Rotation()
{
	//正確には弾丸を移動方向に向かせるプログラム
		//移動速度を↓に入れるとできる。
	float angle = atan2(-m_velocity.x, m_velocity.z);
	m_rotation.SetRotationY(-angle);
	m_modelrender->SetRotation(m_rotation);
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}

void B_normalbullet::Inpacttime()
{
	if (bullettime > 0)
	{
		return;
	}
	DeleteGO(m_collisionObject);
	delete m_modelrender;
	DeleteGO(this);
}


void B_normalbullet::PlayAnimation()
{
	m_modelrender->PlayAnimation(enAnimationClip_Move, 0.1f);
}

void B_normalbullet::Render(RenderContext& rc)
{
	m_modelrender->Draw(rc);
}