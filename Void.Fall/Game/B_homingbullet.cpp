#include "stdafx.h"
#include "B_homingbullet.h"
#include "P_main_Player.h"
#include "collision/CollisionObject.h"
//define
namespace
{
	const Vector3 scale = { 1.5f,1.5f,1.5f }; //大きさ
	const float bullet_spped = 10.0f;         //速度
	const float curvature = 60.0f;            //曲がる強さ
	const float damping = 0.1f;               //減衰
}

bool B_homingbullet::Start()
{

	//アニメーション読み込み
	m_animationclips[enAnimationClip_Move].Load
	("Assets/modelData/A_attack/bullet/homingbullet/homingbullet.tka");
	m_animationclips[enAnimationClip_Move].SetLoopFlag(true);

	//モデル読み込み
	m_modelrender = new ModelRender;
	m_modelrender->Init("Assets/modelData/A_attack/bullet/homingbullet/homingbullet1.tkm",
		m_animationclips, enAnimationClip_Num);

	m_modelrender->SetPosition(m_position);
	m_modelrender->SetScale(scale);

	m_player = FindGO<P_main_Player>("player");

	return true;
}

void B_homingbullet::Update()
{
	//弾丸移動
	Movebullet();
	//弾丸衝突処理
	Inpact();
	//アニメーション
	PlayAnimation();
	//描画処理
	m_modelrender->Update();
}

void B_homingbullet::PlayAnimation()
{
	m_modelrender->PlayAnimation(enAnimationClip_Move, 0.1f);
}

void B_homingbullet::Movebullet()
{
	CalcVelocity(bullet_spped, curvature, damping);
	m_position += m_velocity;
	bullettime -= g_gameTime->GetFrameDeltaTime();
	SetTarget(m_player->m_position);
	m_modelrender->SetPosition(m_position);
}

void B_homingbullet::CalcVelocity(const float speed, const float curvatureRadius,
	const float damping)
{
	float maxCentripetalAccel = speed * speed / curvatureRadius;
	float propulsion = speed * damping;

	Vector3 targetPosition = m_targetPosition;
	Vector3 toTarget = targetPosition - m_position;
	Vector3 vn = m_velocity;
	vn.Normalize();
	float dot = toTarget.Dot(vn);
	Vector3 centripetalAccel = toTarget - (vn * dot);
	float centripetalAccelMagnitude = centripetalAccel.Length();
	if (centripetalAccelMagnitude > 1.0f)
	{
		centripetalAccel /= centripetalAccelMagnitude;
	}
	Vector3 force = centripetalAccel * curvatureRadius;
	force += vn * propulsion;
	force -= m_velocity * damping;
	m_velocity += force * g_gameTime->GetFrameDeltaTime();
}

void B_homingbullet::Inpact()
{
	if (bullettime > 0)
	{
		return;
	}
	DeleteGO(this);
}

void B_homingbullet::Render(RenderContext& rc)
{
	m_modelrender->Draw(rc);
}