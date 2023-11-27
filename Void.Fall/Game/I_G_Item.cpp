#include "stdafx.h"
#include "I_G_Item.h"

bool I_G_Item::Start()
{
	//�G�t�F�N�g�ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(1,
		u"Assets/modelData/A_item/item_G/item.g.efk");

	m_effect = NewGO<EffectEmitter>(1);
	m_effect->Init(1);
	m_effect->SetScale({ 10.0f,10.0f,10.0f });
	m_effect->SetPosition(m_position);
	m_effect->Play();
	//���W
	m_modelrender.SetPosition(m_position);

	//�R���W�����I�u�W�F�N�g���쐬����B
	m_collisionObject = NewGO<CollisionObject>(0);
	//����̃R���W�������쐬����B
	m_collisionObject->CreateSphere(m_position, Quaternion::Identity, 30.0f * m_scale.z);
	m_collisionObject->SetName("Item_col_G");
	m_collisionObject->SetPosition(m_position);
	//�R���W�����I�u�W�F�N�g�������ō폜����Ȃ��悤�ɂ���B
	m_collisionObject->SetIsEnableAutoDelete(false);

	m_player = FindGO<P_main_Player>("player");
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
	return true;
}

void I_G_Item::Update()
{
	//�����蔻�菈��
	Collision();
	//�A�C�e����������
	deleteitem();
	//�`�攻��
	m_modelrender.Update();
}

void I_G_Item::Render(RenderContext& rc)
{
	m_modelrender.Draw(rc);
}