#pragma once
class P_main_Player;
class Game;
class Ob_savepoint : public IGameObject
{
public:
	enum State {
		en_Standby,
		en_save,
	};
	enum EnAnimationClip {
		enAnimationClip_Standby,//棒立ち
		enAnimationClip_Save,
		enAnimationClip_Num,
	};
	bool Start();
	void Render(RenderContext& rc);
	void Update();
	void Collision();										//本体の当たり判定
	void PlayAnimation();
	void ManageState();//遷移処理
	void ProcessSaveStateTransition();//セーブ遷移
	//初期設定系統
	void Setposition(const Vector3& position)//座標
	{
		m_position = position;
	}
	void Setrotarion(const Quaternion& rotation)//回転値
	{
		m_rotation = rotation;
	}
	void Setscale(const Vector3& scale)//座標
	{
		m_scale = scale;
	}
	void SetNum(int num)
	{
		savenum = num;
	}
	void SetsaveVec()
	{
		Vector3 correpos{ 0.0,0.0,-150.0 };
		m_Saveposition = m_position + correpos;
	}
	Vector3 m_position;						//座標
	Quaternion m_rotation;					//回転
	Vector3 m_scale = Vector3::One;			//大きさ
	CollisionObject* m_collisionObject;		//コリジョンオブジェクト。
	Vector3	m_forward = Vector3::AxisZ;		//自身の正面ベクトル。
	P_main_Player* m_player = nullptr;		//プレイヤー
	Game* m_game = nullptr;                 //ゲーム
	ModelRender m_modelrender;//モデルレンダー
	PhysicsStaticObject m_physicsStaticObject;//当たり判定
	Vector3 m_Saveposition;						//セーブポジション
	AnimationClip m_animationclips[enAnimationClip_Num];     //アニメーションクリップ
	State m_state = en_Standby;
	int savenum = 0;
};

