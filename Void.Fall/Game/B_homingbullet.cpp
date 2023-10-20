#include "stdafx.h"
#include "B_homingbullet.h"
#include "P_main_Player.h"
#include "collision/CollisionObject.h"
//define
namespace
{
	const Vector3 scale = { 1.5f,1.5f,1.5f }; //大きさ
	const Vector3 corre = { 0.0f,60.0f,0.0f };//位置修正
	const float bullet_homingtime = 3.0f;     //最大追跡時間
	const float bullet_spped = 10.0f;         //速度
	const float curvature = 25.0f;            //曲がる強さ
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

	//コリジョンオブジェクトを作成する。
	m_collisionObject = NewGO<CollisionObject>(0);
	//球状のコリジョンを作成する。
	m_collisionObject->CreateSphere(m_position, Quaternion::Identity, 20.0f * m_scale.z);
	m_collisionObject->SetName("enemy_attack");
	//コリジョンオブジェクトが自動で削除されないようにする。
	m_collisionObject->SetIsEnableAutoDelete(false);

	m_player = FindGO<P_main_Player>("player");

	return true;
}

void B_homingbullet::Update()
{
	//弾丸移動
	Movebullet();
	//回転処理
	Rotation();
	//弾丸時間消滅処理
	Inpacttime();
	//弾丸対象衝突処理
	Inpacthit();
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
	if (bullettime > bullet_homingtime){
		CalcVelocity(bullet_spped, curvature, damping);
	}
	m_position += m_velocity;
	bullettime -= g_gameTime->GetFrameDeltaTime();
	SetTarget(m_player->m_position);
	m_collisionObject->SetPosition(m_position);
	m_modelrender->SetPosition(m_position);
}

void B_homingbullet::Rotation()
{
	//正確には弾丸を移動方向に向かせるプログラム
			//移動速度を↓に入れるとできる。
	float angle = atan2(-m_velocity.x, m_velocity.z);
	m_rotation.SetRotationY(-angle);
	m_modelrender->SetRotation(m_rotation);
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}

void B_homingbullet::CalcVelocity(const float speed, const float curvatureRadius,
	const float damping)
{
	float maxCentripetalAccel = speed * speed / curvatureRadius;
	float propulsion = speed * damping;

	Vector3 targetPosition = m_targetPosition += corre;
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

void B_homingbullet::Inpacttime()
{
	if (bullettime > 0)
	{
		return;
	}
	DeleteGO(m_collisionObject);
	delete m_modelrender;
	DeleteGO(this);
}

void B_homingbullet::Inpacthit()
{
	if (m_collisionObject->IsHit(m_player->m_charaCon))
	{
		DeleteGO(m_collisionObject);
		delete m_modelrender;
		DeleteGO(this);
	}
}

void B_homingbullet::Render(RenderContext& rc)
{
	m_modelrender->Draw(rc);
}