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
	//���ʏ����ݒ�
	//IEnemy() {}
	void DeleteGoEnemyList()
	{
		m_game = FindGO<Game>("game");
		m_player->m_LockonTF = false;
		m_game->Delete_EnemyVec(m_Vectornum);
		m_game->m_numenemy--;
		if (m_summon_type == Enemy4Sum) {
			m_game->SummonEnemynum--;
		}
		//m_player->m_isTakeAim = false;
	}
	void DeleteGoEnemy()
	{
		DeleteGO(m_collisionObject);//��������
		DeleteGO(this);
	}
	void Setposition(const Vector3& position)//���W
	{
		m_position = position;
	}
	const Vector3& Getposition() const//���W�擾�v���O����
	{
		return m_position;
	}
	void Setrotation(const Quaternion& rotation)//��]�l
	{
		m_rotation = rotation;
	}
	void Setscale(const Vector3& scale)//���W
	{
		m_scale = scale;
	}
	void SetHP(const int hp)//����HP
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
			m_sh = 1;
			m_enemysheld_type = SheldEnemy;
			return;
		}
		return;
	}
	void Sheldeffect()
	{
		if (m_enemysheld_type == SheldEnemy){
			m_effectSH = NewGO<EffectEmitter>(3);
			m_effectSH->Init(3);
			m_effectSH->SetScale({ 10.0f,10.0f,10.0f });
			m_effectSH->SetPosition(m_position);
			m_effectSH->SetRotation(m_rotation);
			m_effectSH->Play();
		}
	}
	void SetVectornum(const int num)//�z��ԍ�
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
		I_G_Item* item_g = NewGO<I_G_Item>(0, "item");
		item_g->Setposition(m_position);
	}
	void EnemyGoEffect()
	{
		//�G�t�F�N�g�ǂݍ���
		m_effect = NewGO<EffectEmitter>(1);
		m_effect->Init(1);
		m_effect->SetScale({ 20.0f,20.0f,20.0f });
		m_effect->SetPosition(m_position);
		m_effect->SetRotation(m_rotation);
		m_effect->Play();
	}
	void Shplay()
	{
		m_effectSH->SetScale({ 20.0f,20.0f,20.0f });
		m_effectSH->SetPosition(m_position);
		m_effectSH->SetRotation(m_rotation);
		m_effectSH->Play();
	}
public:
	EffectEmitter* m_effect = nullptr;		//�G�t�F�N�g(���ݍő�v�f�P)
	EffectEmitter* m_effectSH = nullptr;		//�V�[���h�p�G�t�F�N�g(���ݍő�v�f�P)
	Vector3 m_movespeed;					//�ړ����x
	Quaternion m_rotation;					//��]
	Vector3 m_scale = Vector3::One;			//�傫��
	ModelRender m_modelrender;	//���f�������_�[
	CollisionObject* m_collisionObject;		//�R���W�����I�u�W�F�N�g�B
	P_main_Player* m_player = nullptr;		//�v���C���[
	CharacterController m_charaCon ;        //�L�����R��
	Vector3 m_position;						//���W
	Game* m_game = nullptr;
	EnemyType m_enemy_type = EnemyType_None;    //��ދK��
	SummonType m_summon_type = NormalSum;     //�����^�C�v
	EnemySheld m_enemysheld_type = NormalEnemy; //�G���V�[���h��t�^����Ă��邩
	E_004_enemy* enemy004 = nullptr;
	int m_hp = 0;                           //HP
	int m_sh = 0;							//�V�[���h
	float m_looptimer = 0.0f;								//���[�v�^�C�}�[
	float looptime = 1.16;									//���[�v����
	int m_Vectornum = 0;					//�z��̃i���o�[
	float m_mutekitimer = 0.0f;								//���G�^�C�}�[
	float mutekitime = 0.1f;								//���G����
};

