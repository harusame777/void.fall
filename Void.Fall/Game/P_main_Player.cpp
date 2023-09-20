#include "stdafx.h"
#include "P_main_Player.h"
///////////////////////////////////////////////////////////
#define playerspeed 250.0f
#define playerjamp 400.0f

bool P_main_Player::Start()
{
	m_modelrender = new ModelRender;
	m_modelrender->Init("Assets/modelData/A_testPlayer/testPlayer.tkm");
	m_charaCon.Init(25.0f, 70.0f, m_position);
	return true;
}

void P_main_Player::Update()
{
	//移動処理
	Move();
	//ホバー処理
	Movefry();
	//回転処理
	Rotation();
	//ステートの遷移処理
	ManageState();
	//描画処理
	m_modelrender->Update();

}

void P_main_Player::Render(RenderContext& rc)
{
	m_modelrender->Draw(rc);
}

void P_main_Player::Move()
{
	//移動できない状態であれば、移動処理はしない。
	if (IsEnableMove() == false)
	{
		return;
	}
	m_movespeed.x = 0.0f;
	m_movespeed.z = 0.0f;

	//このフレームの移動量を求める。
	//左スティックの入力量を受け取る。
	float lStick_x = g_pad[0]->GetLStickXF();
	float lStick_y = g_pad[0]->GetLStickYF();
	//カメラの前方方向と右方向を取得。
	Vector3 cameraForward = g_camera3D->GetForward();
	Vector3 cameraRight = g_camera3D->GetRight();
	//XZ平面での前方方向、右方向に変換する。
	cameraForward.y = 0.0f;
	cameraForward.Normalize();
	cameraRight.y = 0.0f;
	cameraRight.Normalize();
	//XZ成分の移動速度をクリア。
	m_movespeed += cameraForward * lStick_y * playerspeed;	//奥方向への移動速度を加算。
	m_movespeed += cameraRight * lStick_x * playerspeed;		//右方向への移動速度を加算。
	if (m_charaCon.IsOnGround() == false && 0 < m_fry && m_fryflag == true && g_pad[0]->IsTrigger(enButtonA))
	{
		m_fryflag = false;
		m_playerstate = enPlayerState_Idlefry;
		return;
	}
	if (g_pad[0]->IsTrigger(enButtonA) //Aボタンが押されたら
		&& m_charaCon.IsOnGround()   //かつ、地面に居たら
		) {
		//ジャンプする。
		m_fryflag = true;
		m_movespeed.y = playerjamp;	//上方向に速度を設定して、
	}
	m_movespeed.y -= 980.0f * g_gameTime->GetFrameDeltaTime();
	//キャラクターコントローラーを使用して、座標を更新。
	m_position = m_charaCon.Execute(m_movespeed, g_gameTime->GetFrameDeltaTime());
	if (m_charaCon.IsOnGround()) {
		//地面についた。
		m_fryflag = false;
		m_movespeed.y = 0.0f;
	}
	Vector3 modelPosition = m_position;
	//ちょっとだけモデルの座標を挙げる。
	modelPosition.y += 2.5f;
	m_modelrender->SetPosition(modelPosition);
}

void P_main_Player::Movefry()
{
	//ホバーできない状態であれば、移動処理はしない。
	if (IsEnableMovefry() == false)
	{
		return;
	}
	m_movespeed.y = 0.0f;
	m_movespeed.x = 0.0f;
	m_movespeed.z = 0.0f;
	//このフレームの移動量を求める。
	//左スティックの入力量を受け取る。
	float lStick_x = g_pad[0]->GetLStickXF();
	float lStick_y = g_pad[0]->GetLStickYF();
	//カメラの前方方向と右方向を取得。
	Vector3 cameraForward = g_camera3D->GetForward();
	Vector3 cameraRight = g_camera3D->GetRight();
	//XZ平面での前方方向、右方向に変換する。
	cameraForward.y = 0.0f;
	cameraForward.Normalize();
	cameraRight.y = 0.0f;
	cameraRight.Normalize();
	//XZ成分の移動速度をクリア。
	m_movespeed += cameraForward * lStick_y * playerspeed;	//奥方向への移動速度を加算。
	m_movespeed += cameraRight * lStick_x * playerspeed;		//右方向への移動速度を加算。
	if (m_fryflag == true && g_pad[0]->IsTrigger(enButtonA) || m_fry == 0)
	{
		m_playerstate = enPlayerState_Idle;
	}
	//キャラクターコントローラーを使用して、座標を更新。
	m_position = m_charaCon.Execute(m_movespeed, g_gameTime->GetFrameDeltaTime());
	Vector3 modelPosition = m_position;
	//ちょっとだけモデルの座標を挙げる。
	modelPosition.y += 2.5f;
	m_modelrender->SetPosition(modelPosition);
}

void P_main_Player::Rotation()
{
	if (fabsf(m_movespeed.x) < 0.001f
		&& fabsf(m_movespeed.z) < 0.001f) {
		//m_moveSpeed.xとm_moveSpeed.zの絶対値がともに0.001以下ということは
		//このフレームではキャラは移動していないので旋回する必要はない。
		return;
	}
	//atan2はtanθの値を角度(ラジアン単位)に変換してくれる関数。
	//m_moveSpeed.x / m_moveSpeed.zの結果はtanθになる。
	//atan2を使用して、角度を求めている。
	//これが回転角度になる。
	float angle = atan2(-m_movespeed.x, m_movespeed.z);
	//atanが返してくる角度はラジアン単位なので
	//SetRotationDegではなくSetRotationを使用する。
	m_rotation.SetRotationY(-angle);

	//回転を設定する。
	m_modelrender->SetRotation(m_rotation);

	//プレイヤーの正面ベクトルを計算する。
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}

void P_main_Player::ManageState()
{
	switch (m_playerstate)
	{
	case enPlayerState_Idle:
	case enPlayerState_Idlefry:
		//待機ステート遷移
		ProcessIdleStateTransition();
		break;
	case enPlayerState_Walk:
	case enPlayerState_Walkfry:
		//歩きステート遷移
		ProcessWalkStateTransition();
		break;
	}
}

void P_main_Player::ProcessIdleStateTransition()
{
	ProcessCommonStateTransition();
}

void P_main_Player::ProcessWalkStateTransition()
{
	ProcessCommonStateTransition();
}

void P_main_Player::ProcessCommonStateTransition()
{
	//xかzの移動速度があったら(スティックの入力があったら)。
	if (fabsf(m_movespeed.x) >= 0.001f || fabsf(m_movespeed.z) >= 0.001f)
	{
		if (m_charaCon.IsOnGround() == true)
		{
			//歩きにする。
			m_playerstate = enPlayerState_Walk;
		}
		else if (m_charaCon.IsOnGround() == false && IsEnableMove() == false)
		{
			m_playerstate = enPlayerState_Walkfry;
			m_fryflag = true;
		}
		return;
	}
	//xとzの移動速度が無かったら(スティックの入力が無かったら)。
	else
	{
		//ステートを待機にする。
		if (m_charaCon.IsOnGround() == true)
		{
			m_playerstate = enPlayerState_Idle;
		}
		else if (m_charaCon.IsOnGround() == false && IsEnableMove() == false)
		{
			m_playerstate = enPlayerState_Idlefry;
		}
		return;
	}
}
