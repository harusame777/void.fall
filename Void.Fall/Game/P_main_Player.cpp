#include "stdafx.h"
#include "P_main_Player.h"
#include "B_normalbullet.h"
#include "collision/CollisionObject.h"
#include "IEnemy.h"
#include "Game.h"
///////////////////////////////////////////////////////////
#define playerspeed 250.0f			//プレイヤースピード
#define playerspeedAvoid 250.0f		//回避スピード
#define playerjamp 400.0f			//プレイヤージャンプ

bool P_main_Player::Start()
{
	//アニメーション読み込み
	m_animationclips[enAnimationClip_Idle].Load("Assets/animData/Player/idle.tka");
	m_animationclips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationclips[enAnimationClip_Walk].Load("Assets/animData/Player/walk.tka");
	m_animationclips[enAnimationClip_Walk].SetLoopFlag(true);
	//enAnimationClip_Attack:アニメーションキーname(magic_attack)
	m_animationclips[enAnimationClip_Attack].Load("Assets/animData/Player/attack1.tka");
	m_animationclips[enAnimationClip_Attack].SetLoopFlag(false);


	m_modelrender = new ModelRender;
	m_modelrender->Init("Assets/modelData/A_testPlayer/RE_Player.tkm", m_animationclips, enAnimationClip_Num);
	m_charaCon.Init(25.0f, 70.0f, m_position);

	//アニメーションイベント用関数設定
	m_modelrender->AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	return true;
}

void P_main_Player::Update()
{
	//移動処理
	Move();
	//回転処理
	Rotation();
	//Lockon位置取得
	Takeaim();
	//ロックオン
	Lockon();
	//アニメーション
	PlayAnimation();
	//ステートの遷移処理
	ManageState();
	//描画処理
	m_modelrender->Update();

}

void P_main_Player::PlayAnimation()
{
	m_modelrender->SetAnimationSpeed(1.0f);
	switch (m_playerstate)
	{
	case enPlayerState_Idle:
		//待機アニメーション
		m_modelrender->PlayAnimation(enAnimationClip_Idle, 0.1f);
		break;
	case enPlayerState_Walk:
		//歩きアニメーション
		m_modelrender->PlayAnimation(enAnimationClip_Walk, 0.1f);
		break;
	case enPlayerState_Attack:
		//攻撃アニメーション
		m_modelrender->PlayAnimation(enAnimationClip_Attack, 0.3f);
		break;
	}
}

void P_main_Player::Render(RenderContext& rc)
{
	m_modelrender->Draw(rc);
}

void P_main_Player::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//アニメーションクリップがmagic_attackだったら
	if (wcscmp(eventName, L"magic_attack") == 0) {
		//normalbulletを作成する。
		auto bullet = NewGO<B_normalbullet>(0);
		//bulletの初期設定など
		bullet->SetPosition(m_position);
		bullet->Setrotation(m_rotation);
		bullet->SetVelocity(m_forward);
		bullet->m_position.y += 80.0f;
		bullet->m_position.z += 10.0f;
	}
}

void P_main_Player::Move()
{
	//移動できない状態であれば、移動処理はしない。
	if (IsEnableMove() == false){
		return;
	}
	//現在のステートが回避ステートだったら
	if (m_playerstate == enPlayerState_Avoidance){
		//回避処理をする
		Avoidance();	
		//回避タイマーをフレーム単位で減らす。
		m_Avoidancetimer -= g_gameTime->GetFrameDeltaTime();
		return;
	}
	//回避していなかったら
	else{
		//回避クールダウンタイマーをフレーム単位で減らす。
		m_Avoidbreaktimer -= g_gameTime->GetFrameDeltaTime();
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
	if (g_pad[0]->IsTrigger(enButtonA) //Aボタンが押されたら
		&& m_charaCon.IsOnGround()   //かつ、地面に居たら
		) {
		//ジャンプする。
		m_movespeed.y = playerjamp;	//上方向に速度を設定して、
	}
	m_movespeed.y -= 980.0f * g_gameTime->GetFrameDeltaTime();
	//キャラクターコントローラーを使用して、座標を更新。
	m_position = m_charaCon.Execute(m_movespeed, g_gameTime->GetFrameDeltaTime());
	if (m_charaCon.IsOnGround()) {
		//地面についた。
		m_movespeed.y = 0.0f;
	}
	Vector3 modelPosition = m_position;
	////ちょっとだけモデルの座標を挙げる。
	modelPosition.y += 2.5f;
	m_modelrender->SetPosition(modelPosition);
}

void P_main_Player::Avoidance()
{
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
	m_movespeed += cameraForward * lStick_y * playerspeedAvoid;	//奥方向への移動速度を加算。
	m_movespeed += cameraRight * lStick_x * playerspeedAvoid;		//右方向への移動速度を加算。
	//キャラクターコントローラーを使用して、座標を更新。
	m_position = m_charaCon.Execute(m_movespeed, g_gameTime->GetFrameDeltaTime());
	Vector3 modelPosition = m_position;
	////ちょっとだけモデルの座標を挙げる。
	modelPosition.y += 2.5f;
	m_modelrender->SetPosition(modelPosition);
}

void P_main_Player::Rotation()
{
	//回避していたら、回転はさせない。
	if (m_playerstate == enPlayerState_Avoidance){
		return;
	}
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
		//待機ステート遷移
		ProcessIdleStateTransition();
		break;
	case enPlayerState_Walk:
		//歩きステート遷移
		ProcessWalkStateTransition();
		break;
	case enPlayerState_Attack:
		//攻撃ステート遷移
		ProcessAttackStateTransition();
		break;
	case enPlayerState_Avoidance:
		//回避ステート遷移
		ProcessAvoidanceStateTransition();
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

void P_main_Player::ProcessAttackStateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelrender->IsPlayingAnimation() == false){
		//ほかのステートに遷移する。
		ProcessCommonStateTransition();
	}
}

void P_main_Player::ProcessAvoidanceStateTransition()
{
	//回避タイマーが0以上だったら
	if (m_Avoidancetimer >= 0)
	{
		//ほかのステートに遷移する。
		ProcessCommonStateTransition();
	}
	//回避タイマーが0以下だったら
	else
	{
		//ステートを待機ステートにして
		m_playerstate = enPlayerState_Idle;
		//回避クールダウンタイマーを初期化する。
		m_Avoidbreaktimer = Avoidbreaktime;
	}
}

void P_main_Player::ProcessCommonStateTransition()
{
	//現在のステートが回避だったら
	if (m_playerstate == enPlayerState_Avoidance)
	{
		//(ほかの処理をさせないため)return;
		return;
	}
	//Bボタンが押されたら
	if (g_pad[0]->IsTrigger(enButtonB)){
		//attackステートにする。
		m_playerstate = enPlayerState_Attack;
		return;
	}
	//回避クールダウンが0以下で
	if (m_Avoidbreaktimer <= 0){
		//Yボタンが押されたら
		if (g_pad[0]->IsTrigger(enButtonY)) {
			//回避ステートにして
			m_playerstate = enPlayerState_Avoidance;
			//回避タイマーを初期値にする
			m_Avoidancetimer = Avoidancetime;
			return;
		}
	}
	//xかzの移動速度があったら(スティックの入力があったら)。
	if (fabsf(m_movespeed.x) >= 0.001f || fabsf(m_movespeed.z) >= 0.001f){
		//歩きステートにする
		m_playerstate = enPlayerState_Walk;
		return;
	}
	//xとzの移動速度が無かったら(スティックの入力が無かったら)。
	else{
		//待機ステートにする。
		m_playerstate = enPlayerState_Idle;
		return;
	}
}

void P_main_Player::Takeaim()
{
	m_game->enemyList[]
}

void P_main_Player::Lockon()
{

}
