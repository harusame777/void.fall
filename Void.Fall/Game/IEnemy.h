#pragma once
#include "Game.h"
#include "P_main_Player.h"
#include "collision/CollisionObject.h"
#include "graphics/effect/EffectEmitter.h"
class Game;
//class P_main_Player;
class IEnemy : public IGameObject
{
public:
	//共通初期設定
	//IEnemy() {}
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
public:
	IEnemy* m_ienemy = nullptr;
	std::vector<Vector3*> m_enemyPositionListIenemy;
	Vector3 m_movespeed;					//移動速度
	Quaternion m_rotation;					//回転
	Vector3 m_scale = Vector3::One;			//大きさ
	ModelRender* m_modelrender = nullptr;	//モデルレンダー
	CollisionObject* m_collisionObject;		//コリジョンオブジェクト。
	P_main_Player* m_player = nullptr;		//プレイヤー
	CharacterController m_charaCon ;        //キャラコン
	Vector3 m_position;						//座標
	Game* m_game = nullptr;
	int m_numenemy = 0;
	int m_hp = 0;                           //HP

};

