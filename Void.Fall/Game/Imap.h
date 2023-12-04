#pragma once
class Imap : public IGameObject
{
public:
	enum mapType {
		en_typeNone,
		en_Start,
		en_Goal,
		en_oneOut,
		en_twoOut,
		en_threeOut,
		en_Outend
	};
	enum mapZrot
	{
		en_rotNone,
		en_0rot,
		en_90rot,
		en_180rot,
		en_270rot,
	};
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
	void SetMapType(const mapType maptype)
	{
		m_mapType = maptype;
	}
	float rot0 = 0;
	float rot90 = 90;
	float rot180 = 180;
	float rot270 = 270;
	void SetMapRotZ(const mapZrot mapzrot)
	{
		m_maprot = mapzrot;
		switch (m_maprot){
		case Imap::en_0rot:
			m_rotation.z = rot0;
			break;
		case Imap::en_90rot:
			m_rotation.z = rot90;
			break;
		case Imap::en_180rot:
			m_rotation.z = rot180;
			break;
		case Imap::en_270rot:
			m_rotation.z = rot270;
			break;
		}
	}
	void SetPhysics(){
		//当たり判定作成
		m_physicsStaticObject.CreateFromModel(m_modelrender.GetModel(), m_modelrender.GetModel().GetWorldMatrix());
	}
	//メンバ関数宣言
	mapType m_mapType = en_typeNone;
	mapZrot m_maprot = en_rotNone;
	Vector3 m_position;//座標
	Vector3 m_scale = Vector3::One;//大きさ
	Quaternion m_rotation;//回転
	ModelRender m_modelrender;//モデルレンダー
	PhysicsStaticObject m_physicsStaticObject;//当たり判定
};

