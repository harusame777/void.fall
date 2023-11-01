#include "stdafx.h"
#include "Ca_maincamera.h"

#include "P_main_Player.h"

bool Ca_maincamera::Start()
{
	//注視点から視点までのベクトルを設定。
	m_Campos.Set(0.0f, 100.0f, -200.0f);
	m_player = FindGO<P_main_Player>("player");
	//ばねカメラの初期化。
	m_springCamera.Init(
		*g_camera3D,		//ばねカメラの処理を行うカメラを指定する。
		1000.0f,			//カメラの移動速度の最大値。
		true,				//カメラと地形とのあたり判定を取るかどうかのフラグ。trueだとあたり判定を行う。
		5.0f				//カメラに設定される球体コリジョンの半径。第３引数がtrueの時に有効になる。
	);
	return true;
}

void Ca_maincamera::Update()
{
	if (m_player->IsLockOn() == false)
	{
		//位置更新
		CamposUpdate();
	}
	else
	{
		ProcessLockOn();
	}
	//カメラ更新
	m_springCamera.Update();
}

void Ca_maincamera::CamposUpdate()
{

	//カメラを更新。
	//注視点を計算する。
	Vector3 target = m_player->Getposition();
	//プレイヤの足元からちょっと上を注視点とする。
	target.y += 80.0f;
	target += g_camera3D->GetForward() * 20.0f;

	Vector3 toCameraPosOld = m_Campos;
	//パッドの入力を使ってカメラを回す。
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();
	//Y軸周りの回転
	Quaternion qRot;
	qRot.SetRotationDeg(Vector3::AxisY, 1.5f * x);
	qRot.Apply(m_Campos);
	//X軸周りの回転。
	Vector3 axisX;
	axisX.Cross(Vector3::AxisY, m_Campos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, 1.5f * y);
	qRot.Apply(m_Campos);
	//カメラの回転の上限をチェックする。
	//注視点から視点までのベクトルを正規化する。
	//正規化すると、ベクトルの大きさが１になる。
	//大きさが１になるということは、ベクトルから強さがなくなり、方向のみの情報となるということ。
	Vector3 toPosDir = m_Campos;
	toPosDir.Normalize();
	if (toPosDir.y < -0.5f) {
		//カメラが上向きすぎ。
		m_Campos = toCameraPosOld;
	}
	else if (toPosDir.y > 0.8f) {
		//カメラが下向きすぎ。
		m_Campos = toCameraPosOld;
	}


	//視点を計算する。
	Vector3 pos = target + m_Campos;

	//バネカメラに注視点と視点を設定する。
	m_springCamera.SetPosition(pos);
	m_springCamera.SetTarget(target);
}

void Ca_maincamera::ProcessLockOn()
{
	Vector3 target = m_player->GetTargetPosition();
	Vector3 toPos = target - m_player->Getposition();
	float r = toPos.Length() + 200.0f;
	toPos.y = 0.0f;
	toPos.Normalize();
	Vector3 position = target - toPos * r;
	position.y = m_player->Getposition().y + 200.0f;

	m_springCamera.SetPosition(position);
	m_springCamera.SetTarget(target);
}