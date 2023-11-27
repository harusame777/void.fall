#pragma once
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
	//���ʏ����ݒ�
	//IEnemy() {}
	void DeleteGoEnemyList()
	{
		m_game->Delete_EnemyVec(m_Vectornum);
		m_game->m_numenemy--;
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
	void SetVectornum(const int num)//�z��ԍ�
	{
		m_Vectornum = num;
	}
	void ItemDrop()
	{
		I_G_Item* item_g = NewGO<I_G_Item>(0, "itemg");
		item_g->Setposition(m_game->m_EnemyList[m_Vectornum]->m_position);
	}
public:
	Vector3 m_movespeed;					//�ړ����x
	Quaternion m_rotation;					//��]
	Vector3 m_scale = Vector3::One;			//�傫��
	ModelRender* m_modelrender = nullptr;	//���f�������_�[
	CollisionObject* m_collisionObject;		//�R���W�����I�u�W�F�N�g�B
	P_main_Player* m_player = nullptr;		//�v���C���[
	CharacterController m_charaCon ;        //�L�����R��
	Vector3 m_position;						//���W
	Game* m_game = nullptr;
	int m_hp = 0;                           //HP
	int m_Vectornum = 0;					//�z��̃i���o�[

};

