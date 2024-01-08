#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Game.h"
#include "P_main_Player.h"
#include "collision/CollisionObject.h"
#include "graphics/effect/EffectEmitter.h"
#include "I_G_Item.h"
class Game;
//class P_main_Player
class IEnemy : public IGameObject
{
public:
	enum EnemySheld {
		NormalEnemy,
		SheldEnemy,
	};
	enum SummonType {
		NormalSum,
		Enemy4Sum
	};
	enum EnemyType{
		en_enemy001,
		en_enemy002,
		en_enemy003,
		EnemyType_Num,
		EnemyType_None
	};
public:
	//共通初期設定
	//IEnemy() {}
	void DeleteGoEnemyList()
	{
		m_game = FindGO<Game>("game");
		m_game->Delete_EnemyVec(m_Vectornum);
		m_game->m_numenemy--;
		if (m_summon_type == Enemy4Sum) {
			m_game->SummonEnemynum--;
		}
		m_player->m_isTakeAim = false;
	}
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
	void SetSHEnum(const EnemySheld enemysheld)
	{
		if (enemysheld == SheldEnemy){
			m_sh = 3;
			m_enemysheld_type = SheldEnemy;
			return;
		}
		m_enemysheld_type = NormalEnemy;
		return;
	}
	void SetSH()
	{
		m_sh = 3;
		m_enemysheld_type = SheldEnemy;
	}
	bool SheldCon()
	{
		if (m_sh > 0){
			return true;
		}
		else{
			return false;
		}
	}
	void SheldRand() {
		srand((unsigned int)time(NULL));
		int sheldrand = rand() % 100 + 1;
		if (sheldrand >= 70){
			m_sh = 3;
			return;
		}
		return;
	}
	void SetVectornum(const int num)//配列番号
	{
		m_Vectornum = num;
	}
	void SetEnemyType(const EnemyType enemytype)
	{
		m_enemy_type = enemytype;
	}
	void SetSummonType(const SummonType summontype)
	{
		m_summon_type = summontype;
	}
	void ItemDrop()
	{
		I_G_Item* item_g = NewGO<I_G_Item>(0, "itemg");
		item_g->Setposition(m_position);
	}
	void EnemyGoEffect()
	{
		//エフェクト読み込み
		EffectEngine::GetInstance()->ResistEffect(1, u"Assets/modelData/Enemy/effect/enemyGo.efk");
		m_effect = NewGO<EffectEmitter>(1);
		m_effect->Init(1);
		m_effect->SetScale({ 20.0f,20.0f,20.0f });
		m_effect->SetPosition(m_position);
		m_effect->SetRotation(m_rotation);
		m_effect->Play();
	}
public:
	EffectEmitter* m_effect = nullptr;		//エフェクト(現在最大要素１)
	Vector3 m_movespeed;					//移動速度
	Quaternion m_rotation;					//回転
	Vector3 m_scale = Vector3::One;			//大きさ
	ModelRender m_modelrender;	//モデルレンダー
	CollisionObject* m_collisionObject;		//コリジョンオブジェクト。
	P_main_Player* m_player = nullptr;		//プレイヤー
	CharacterController m_charaCon ;        //キャラコン
	Vector3 m_position;						//座標
	Game* m_game = nullptr;
	EnemyType m_enemy_type = EnemyType_None;    //種類規定
	SummonType m_summon_type = NormalSum;     //召喚タイプ
	EnemySheld m_enemysheld_type = NormalEnemy; //敵がシールドを付与されているか
	E_004_enemy* enemy004 = nullptr;
	int m_hp = 0;                           //HP
	int m_sh = 0;							//シールド
	int m_Vectornum = 0;					//配列のナンバー

};

