#include "stdafx.h"
#include "P_main_Player.h"
#include "B_normalbullet.h"
#include "B_homingbullet.h"
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
	m_animationclips[enAnimationClip_ReceiveDamage].Load("Assets/animData/Player/receivedamage.tka");
	m_animationclips[enAnimationClip_ReceiveDamage].SetLoopFlag(false);
	m_animationclips[enAnimationClip_Down].Load("Assets/animData/Player/down.tka");
	m_animationclips[enAnimationClip_Down].SetLoopFlag(false);
	//enAnimationClip_Attack:アニメーションキーname(attack_start)
	//enAnimationClip_Attack:アニメーションキーname(attack_end)
	m_animationclips[enAnimationClip_Attacknear].Load("Assets/animData/Player/attack2.tka");
	m_animationclips[enAnimationClip_Attacknear].SetLoopFlag(false);


	m_modelrender = new ModelRender;
	m_modelrender->Init("Assets/modelData/A_testPlayer/RE_Player.tkm", m_animationclips, enAnimationClip_Num);
	m_charaCon.Init(25.0f, 70.0f, m_position);

	m_spriterender.Init("Assets/sprite/testlock/lock.dds", 200, 200);
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
	//当たり判定処理
	Collision();
	//Lockon位置取得
	Takeaim();
	//ロックオン
	Lockon();
	//ロックオン選択
	LockonLR();
	//MP
	MP();
	//アニメーション
	PlayAnimation();
	//ステートの遷移処理
	ManageState();
	//描画処理
	m_modelrender->Update();
	m_spriterender.Update();

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
	case enPlayerState_Attacknear:
		m_modelrender->PlayAnimation(enAnimationClip_Attacknear, 0.1f);
		break;
	case enPlayerState_Avoidance:
		//回避アニメーション
		m_modelrender->PlayAnimation(enAnimationClip_Avoidance, 0.1f);
		break;
	case enPlayerState_ReceiveDamage:
		//被ダメアニメーション
		m_modelrender->PlayAnimation(enAnimationClip_ReceiveDamage, 0.1f);
		break;
	case enPlayerState_Down:
		//ダウンアニメーション
		m_modelrender->PlayAnimation(enAnimationClip_Down, 0.1f);
		break;
	}
}

void P_main_Player::Render(RenderContext& rc)
{
	m_modelrender->Draw(rc);
	if (m_isTakeAim == false)
	{
		return;
	}
	m_spriterender.Draw(rc);
}

void P_main_Player::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//アニメーションクリップがmagic_attackだったら
	if (wcscmp(eventName, L"magic_attack") == 0) {
		if (0 < m_mp){
			if (m_isLockOn == true) {
				auto bullet = NewGO<B_homingbullet>(0);
				bullet->SetPosition(m_position);
				bullet->SetVelocity(m_forward * 10);
				bullet->m_position.y += 80.0f;
				bullet->m_position.z += 10.0f;
				bullet->SetEnShooter(B_homingbullet::enShooter_Player);
				m_mp--;
				m_mpRec = mpRecReset;
				mpRecgo = true;
				return;
			}
			//normalbulletを作成する。
			auto bullet = NewGO<B_normalbullet>(0);
			//bulletの初期設定など
			bullet->SetPosition(m_position);
			bullet->Setrotation(m_rotation);
			bullet->SetVelocity(m_forward);
			bullet->m_position.y += 80.0f;
			bullet->m_position.z += 10.0f;
			m_mp--;
			m_mpRec = mpRecReset;
			mpRecgo = true;
		}
	}
}

void P_main_Player::Move()
{
	//移動できない状態であれば、移動処理はしない。
	if (IsEnableMove() == true){
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
	if (g_pad[0]->IsTrigger(enButtonX) //Aボタンが押されたら
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
	//if (m_isLockOn == true){
	//	if (m_playerstate == enPlayerState_Attack){
	//		AttackRotation();
	//		return;
	//	}
	//}
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

void P_main_Player::AttackRotation()
{
	//Vector3 diff = m_position - m_targetPosition;
	//diff.Normalize();
	//Quaternion attackrotation;
	//attackrotation.Apply(diff);
	//m_rotation.SetRotationY(attackrotation.y);
	//m_modelrender->SetRotation(attackrotation);
	//m_forward = Vector3::AxisZ;
	//m_rotation.Apply(m_forward);
}

void P_main_Player::Collision()
{
	//無敵時間中は処理しない
	if (m_mutekitimer > 0)
	{
		m_mutekitimer -= g_gameTime->GetFrameDeltaTime();
		return;
	}
	//被ダメージ、回避中、あるいはダウンステートの時は。
//当たり判定処理はしない。
	if (m_playerstate == enPlayerState_ReceiveDamage ||
		m_playerstate == enPlayerState_Avoidance || 
		m_playerstate == enPlayerState_Down)
	{
		return;
	}
	//エネミーの攻撃用のコリジョンを取得する。
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("enemy_attack");
	//コリジョンの配列をfor文で回す。
	for (auto collision : collisions)
	{
		//コリジョンとキャラコンが衝突したら。
		if (collision->IsHit(m_charaCon)){
			//HPを1減らす。
			m_hp -= 1;
			//HPが0になったら。
			if (m_hp == 0) {
				//ダウンステートに遷移する。
				m_playerstate = enPlayerState_Down;
			}
			else {
				//被ダメージステートに遷移する。
				m_playerstate = enPlayerState_ReceiveDamage;
			}
		}
	}
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
	case enPlayerState_ReceiveDamage:
		//被ダメ遷移
		ProcessReceiveDamageStateTransition();
		break;
	case enPlayerState_Down:
		//ダウン遷移
		ProcessDownStateTransition();
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
		//スキンを通常状態に変更して
		NormalTex();
		//ステートを待機ステートにして
		m_playerstate = enPlayerState_Idle;
		//回避クールダウンタイマーを初期化する。
		m_Avoidbreaktimer = Avoidbreaktime;
	}
}

void P_main_Player::ProcessReceiveDamageStateTransition()
{
	//被ダメージアニメーションの再生が終わったら。
	if (m_modelrender->IsPlayingAnimation() == false)
	{
		//無敵タイマーを初期化
		m_mutekitimer = mutekitime;
		//ステートを待機ステートにして
		m_playerstate = enPlayerState_Idle;
		//ステートを遷移する。
		ProcessCommonStateTransition();
	}
}

void P_main_Player::ProcessDownStateTransition()
{
	//ダウンアニメーションの再生が終わったら。
	if (m_modelrender->IsPlayingAnimation() == false)
	{
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
	if (g_pad[0]->IsTrigger(enButtonRB1)){
		//attackステートにする。
		m_playerstate = enPlayerState_Attack;
		return;
	}
	//回避クールダウンが0以下で
	if (m_Avoidbreaktimer <= 0){
		//スティックからの入力があり
		if (fabsf(m_movespeed.x) >= 0.001f || fabsf(m_movespeed.z) >= 0.001f) {
			//Yボタンが押されたら
			if (g_pad[0]->IsTrigger(enButtonA)) {
				//スキンを回避状態に変更し
				AvoidanceTex();
				//回避ステートにして
				m_playerstate = enPlayerState_Avoidance;
				//回避タイマーを初期値にする
				m_Avoidancetimer = Avoidancetime;
				return;
			}
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

void P_main_Player::NormalTex()
{
	delete m_modelrender;
	//アニメーション読み込み
	m_animationclips[enAnimationClip_Idle].Load("Assets/animData/Player/idle.tka");
	m_animationclips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationclips[enAnimationClip_Walk].Load("Assets/animData/Player/walk.tka");
	m_animationclips[enAnimationClip_Walk].SetLoopFlag(true);
	//enAnimationClip_Attack:アニメーションキーname(magic_attack)
	m_animationclips[enAnimationClip_Attack].Load("Assets/animData/Player/attack1.tka");
	m_animationclips[enAnimationClip_Attack].SetLoopFlag(false);
	m_animationclips[enAnimationClip_ReceiveDamage].Load("Assets/animData/Player/receivedamage.tka");
	m_animationclips[enAnimationClip_ReceiveDamage].SetLoopFlag(false);
	m_animationclips[enAnimationClip_Down].Load("Assets/animData/Player/down.tka");
	m_animationclips[enAnimationClip_Down].SetLoopFlag(false);
	//enAnimationClip_Attack:アニメーションキーname(attack_start)
	//enAnimationClip_Attack:アニメーションキーname(attack_end)
	m_animationclips[enAnimationClip_Attacknear].Load("Assets/animData/Player/attack2.tka");
	m_animationclips[enAnimationClip_Attacknear].SetLoopFlag(false);
	m_modelrender = new ModelRender;
	m_modelrender->Init("Assets/modelData/A_testPlayer/RE_Player.tkm",
		m_animationclips, enAnimationClip_Num);
	m_modelrender->SetRotation(m_rotation);
	m_modelrender->SetPosition(m_position);
	//アニメーションイベント用関数設定
	m_modelrender->AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

}

void P_main_Player::AvoidanceTex()
{
	delete m_modelrender;
	m_animationclips_sub[enAnimationClip_Avoidance].Load("Assets/animData/Player/avoidance.tka");
	m_animationclips_sub[enAnimationClip_Avoidance].SetLoopFlag(true);
	m_modelrender = new ModelRender;
	m_modelrender->Init("Assets/modelData/A_testPlayer/RE_Player_Avoid.tkm",
		m_animationclips_sub, enAnimationClip_sub_Num);
	m_modelrender->SetRotation(m_rotation);
	m_modelrender->SetPosition(m_position);
}

void P_main_Player::Takeaim()
{
	//エネミーの数が0だったら処理しない。
	if (m_numenemy == 0){
		enemypossub = { 1000.0f,1000.0f,1000.0f };
		return;
	}
	//エネミー距離計算
	//エネミーの数が１以下であれば比べる必要はない。
	if (m_numenemy > 1)
	{
		//ロックオンしていなかったら
		if (m_isLockOn == false) {
			//繰り返しの回数は現在のエネミーの数-1する。
			for (ListnumA = 0; ListnumA < m_numenemy - 1; ListnumA++) {
				//繰り返しのint iの数字の配列のエネミーポジションの
				//位置を入れたローカル変数えねぽす１を定義する。
				Vector3 enemypos1 = *m_enemyPositionList[ListnumA];
				//繰り返しのint i+1の数字の配列のエネミーポジションの
				//位置を入れたローカル変数えねぽす２を定義する(どちらが近いか比べるため)。
				Vector3 enemypos2 = *m_enemyPositionList[ListnumA + 1];
				//えねぽす１の位置とプレイヤーの位置の距離を計算した
				//ローカル変数diff1を定義する。
				Vector3 diff1 = enemypos1 - m_position;
				//えねぽす２の位置とプレイヤーの位置の距離を計算した
				//ローカル変数diff2を定義する。
				Vector3 diff2 = enemypos2 - m_position;
				//diff1とdiff2の距離を比べた後に小さかったほうの位置を格納してある
				//えねぽすさぶ(初期値すべて1000.0f、この処理後は
				//一番小さい距離のポジションが入る)の距離を計算した
				//ローカル変数diff3を定義する。
				Vector3 diff3 = enemypossub - m_position;
				//diff1とdiff2の距離を比べる。
				//diff1の方が小さかったら、
				if (diff1.Length() < diff2.Length()) {
					//diff3より小さかったら、
					if (diff1.Length() < diff3.Length()) {
						//えねぽす１をえねぽすさぶに代入する。
						enemypossub = enemypos1;
						ListnumB = ListnumA;
					}
				}
				//そうでなかったら、
				else {
					//diff3より小さかったら、
					if (diff2.Length() < diff3.Length()) {
						//えねぽす２をえねぽすさぶに代入する。
						enemypossub = enemypos2;
						ListnumB = ListnumA + 1;
					}
				}
			}
		}
		//ロックオンしていたら
		else
		{
			Vector3 enemypos3 = *m_enemyPositionList[ListnumB];
			enemypossub = enemypos3;
		}
	}
	//エネミーの数が1以下であれば、
	else{
		//配列0番の位置をえねぽすさぶに代入する。
		Vector3 enemypos3 = *m_enemyPositionList[m_numenemy - 1];
		enemypossub = enemypos3;
	}
	//計算が終了した後の一番近い位置のエネミーの位置と
	//プレイヤーの位置の距離を計算したローカル変数diff4を
	//定義する。
	Vector3 diff4 = enemypossub - m_position;
	//そのdiff4の距離が700.0fよりも大きいなら、
	if (diff4.Length() >= 1000.0f){
		//ロックオンしない。
		m_isTakeAim = false;
		return;
	}

	//位置をちょっと上げる
	enemypossub.y = 60.0f;
	//エネミーの位置をそのまま正規化したらマズいので
	//エネミーとプレイヤー(以下略)を計算した
	//ローカル変数enemyangleを定義してそっちを正規化する。
	Vector3 enemyangle = enemypossub - m_position;
	enemyangle.Normalize();

	float angle = acosf(m_forward.Dot(enemyangle));

	//プレイヤーの正面ベクトルと。
	//プレイヤーからエネミーに向かうベクトルの。
	//角度が90度以上かつ。
	//ロックオン状態でなかったら。
	//ターゲッティングしない。
	if (angle > Math::PI / 2 && m_isLockOn == false)
	{
		m_isTakeAim = false;
		return;
	}
	m_isTakeAim = true;

	Vector2 screenPosition;
	g_camera3D->CalcScreenPositionFromWorldPosition(screenPosition, enemypossub);
	m_spriterender.SetPosition(Vector3(screenPosition.x, screenPosition.y, 0.0f));
	m_targetPosition = enemypossub;
}

void P_main_Player::LockonLR()
{
	if (m_isLockOn){
		if (g_pad[0]->IsTrigger(enButtonRight)){
			LockonLRDis(en_R);
		}
		if (g_pad[0]->IsTrigger(enButtonLeft)) {
			LockonLRDis(en_L);
		}
	}
}

void P_main_Player::LockonLRDis(LockonLRen LR)
{
	switch (LR)
	{
	case P_main_Player::en_R:
		for (int i = 0; i < m_numenemy; i++){
			if (ListnumB == m_numenemy - 1) {
				ListnumB = 0;
			}
			else{
				ListnumB++;
			}
			Vector3 enemypos1 = *m_enemyPositionList[ListnumB];
			Vector3 diff1 = enemypos1 - m_position;
			if (diff1.Length() <= 1000.f) {
				enemypossub = enemypos1;
				break;
			}
		}
		break;
	case P_main_Player::en_L:
		for (int i = 0; i < m_numenemy; i++) {
			if (ListnumB == 0) {
				ListnumB = m_numenemy - 1;
			}
			else {
				ListnumB--;
			}
			Vector3 enemypos1 = *m_enemyPositionList[ListnumB];
			Vector3 diff1 = enemypos1 - m_position;
			if (diff1.Length() <= 1000.f) {
				enemypossub = enemypos1;
				break;
			}
		}
		break;
	}
}

void P_main_Player::Lockon()
{
	//ターゲッティングがされていなかったら。
//ロックオンしない。
	if (m_isTakeAim == false)
	{
		m_isLockOn = false;
		return;
	}

	if (g_pad[0]->IsTrigger(enButtonLB1))
	{
		//ロックオンしていなければ。
		if (m_isLockOn == false)
		{
			//ロックオンする。
			m_isLockOn = true;
		}
		//ロックオンしていれば。
		else
		{
			//ロックオンしない。
			m_isLockOn = false;
		}
	}
}

void P_main_Player::MP()
{
	if (mpRecgo)
	{
		if (m_mp == 3) {
			m_mpRec = mpRecReset;
			mpRecgo = false;
			return;
		}
		m_mpRec += g_gameTime->GetFrameDeltaTime();
		if (m_mpRec > 3.0f) {
			m_mp++;
			m_mpRec = mpRecReset;
		}
	}
}
