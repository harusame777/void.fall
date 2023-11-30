#pragma once
class Imap : public IGameObject
{
public:
	//関数宣言
	virtual bool Start() = 0;
	void Update(){
		//描画更新
		m_modelrender.Update();
	}
	void Render(RenderContext& rc){
		//描画処理
		m_modelrender.Draw(rc);
	}
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
	//メンバ関数宣言
	Vector3 m_position;//座標
	Vector3 m_scale = Vector3::One;//大きさ
	Quaternion m_rotation;//回転
	ModelRender m_modelrender;//モデルレンダー
	PhysicsStaticObject m_physicsStaticObject;//当たり判定
};

