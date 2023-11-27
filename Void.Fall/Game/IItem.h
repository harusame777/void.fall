#pragma once
#include "collision/CollisionObject.h"
#include "graphics/effect/EffectEmitter.h"
#include "P_main_Player.h"
class IItem : public IGameObject //アイテムの基底クラスIItem
{
public:
	void Setposition(const Vector3& position)//座標
	{
		m_position = position;
	}
	//当たり判定処理
	void Collision()
	{
		if (m_collisionObject->IsHit(m_player->m_charaCon)) {
			if (m_isDelete == false) {
				m_isDelete = true;	//deletebulletのif文が通るようにする。
				m_deleteTimer = deletetimer; //deletetimerは現在0.2f。
			}
		}
	}
	//消去ディレイ
	void deleteitem()
	{
		if (m_isDelete) {
			m_deleteTimer -= g_gameTime->GetFrameDeltaTime(); //deletetimerを1フレームずつ
			//減らす。
				//↓タイマーがゼロになったら。(deletetimerより0の方が大きくなったら)
			if (m_deleteTimer <= 0.0f) {
				DeleteGO(m_collisionObject);//消去処理
				DeleteGO(this);
			}
		}
	}
	EffectEmitter* m_effect = nullptr;		//エフェクト
	Vector3 m_position;						//座標
	Quaternion m_rotation;					//回転
	Vector3 m_scale = Vector3::One;			//大きさ
	ModelRender  m_modelrender;				//モデルレンダー
	CollisionObject* m_collisionObject;		//コリジョンオブジェクト。
	Vector3	m_forward = Vector3::AxisZ;		//自身の正面ベクトル。
	P_main_Player* m_player = nullptr;		//プレイヤー
	float m_deleteTimer = 0.0f;								//消去ディレイ
	bool m_isDelete = false;								//消去ディレイに使うやつ
	const float deletetimer = 0.1f;							//ディレイタイマー
};

