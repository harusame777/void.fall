#pragma once
#include <time.h>
#include <stdlib.h>
class Imap : public IGameObject
{
public:
	//�֐��錾
	virtual bool Start() = 0;
	void Update(){
		//�`��X�V
		m_modelrender.Update();
	}
	void Render(RenderContext& rc){
		//�`�揈��
		m_modelrender.Draw(rc);
	}
	//�����ݒ�n��
	void Setposition(const Vector3& position)//���W
	{
		m_position = position;
	}
	void Setrotarion(const Quaternion& rotation)//��]�l
	{
		m_rotation = rotation;
	}
	void Setscale(const Vector3& scale)//���W
	{
		m_scale = scale;
	}
	void SetPhysics(){
		//�����蔻��쐬
		m_physicsStaticObject.CreateFromModel(m_modelrender.GetModel(), m_modelrender.GetModel().GetWorldMatrix());
	}
	//�����o�֐��錾
	Vector3 m_position;//���W
	Vector3 m_scale = Vector3::One;//�傫��
	Quaternion m_rotation;//��]
	ModelRender m_modelrender;//���f�������_�[
	PhysicsStaticObject m_physicsStaticObject;//�����蔻��
	static int boxnum;
};

