#pragma once
#include "IEnemy.h"
class E_002_enemy : public IEnemy
{
public:
	//アニメーション類/////////////////////////////////////////
//アニメーションステート
	enum EnAnimationClip {
		enAnimationClip_Idle,//棒立ち
		enAnimationClip_Num//アニメーション数
	};
	E_002_enemy() {};
	~E_002_enemy() {};
	bool Start();
	void Update();                                         //アップデート
	void Render(RenderContext& rc);                        //モデルレンダー
	///////////////////////////////////////////////////////////
	//初期設定系統
	void Setposition(const Vector3& position)//座標
	{
		m_position = position;
	}
	const Vector3& Getposition() const//座標取得プログラム
	{
		return m_position;
	}
	void Setrotation(const Quaternion& rotation)//回転値
	{
		m_rotation = rotation;
	}
	void Setscale(const Vector3& scale)//座標
	{
		m_scale = scale;
	}
	void SetHP(const int hp)//初期HP
	{
		m_hp = hp;
	}
	//void SetALLposition()
	//{
	//	m_enemyList[m_game->m_numenemy] = m_enemy002;
	//}
	//メンバ関数宣言
	E_002_enemy* m_enemy002 = nullptr;
	AnimationClip m_animationclips[enAnimationClip_Num];     //アニメーションクリップ
	ModelRender* m_modelrender = nullptr;					//モデルレンダー
	Vector3 m_position;										//座標
	Vector3 m_scale = Vector3::One;							//大きさ
	Quaternion m_rotation;									//回転
	CharacterController	m_charaCon;							//キャラクターコントローラー。
	int m_hp = 0;                                           //HP
};

