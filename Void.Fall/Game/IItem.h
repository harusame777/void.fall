#pragma once
#include "collision/CollisionObject.h"
#include "graphics/effect/EffectEmitter.h"
#include "P_main_Player.h"
class IItem : public IGameObject //�A�C�e���̊��N���XIItem
{
public:
	void Setposition(const Vector3& position)//���W
	{
		m_position = position;
	}
	//�����蔻�菈��
	void Collision()
	{
		if (m_collisionObject->IsHit(m_player->m_charaCon)) {
			if (m_isDelete == false) {
				m_isDelete = true;	//deletebullet��if�����ʂ�悤�ɂ���B
				m_deleteTimer = deletetimer; //deletetimer�͌���0.2f�B
			}
		}
	}
	//�����f�B���C
	void deleteitem()
	{
		if (m_isDelete) {
			m_deleteTimer -= g_gameTime->GetFrameDeltaTime(); //deletetimer��1�t���[������
			//���炷�B
				//���^�C�}�[���[���ɂȂ�����B(deletetimer���0�̕����傫���Ȃ�����)
			if (m_deleteTimer <= 0.0f) {
				DeleteGO(m_collisionObject);//��������
				DeleteGO(this);
			}
		}
	}
	EffectEmitter* m_effect = nullptr;		//�G�t�F�N�g
	Vector3 m_position;						//���W
	Quaternion m_rotation;					//��]
	Vector3 m_scale = Vector3::One;			//�傫��
	ModelRender  m_modelrender;				//���f�������_�[
	CollisionObject* m_collisionObject;		//�R���W�����I�u�W�F�N�g�B
	Vector3	m_forward = Vector3::AxisZ;		//���g�̐��ʃx�N�g���B
	P_main_Player* m_player = nullptr;		//�v���C���[
	float m_deleteTimer = 0.0f;								//�����f�B���C
	bool m_isDelete = false;								//�����f�B���C�Ɏg�����
	const float deletetimer = 0.1f;							//�f�B���C�^�C�}�[
};

