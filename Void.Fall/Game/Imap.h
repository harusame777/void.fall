#pragma once
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
	//�����o�֐��錾
	Vector3 m_position;//���W
	Vector3 m_scale = Vector3::One;//�傫��
	Quaternion m_rotation;//��]
	ModelRender m_modelrender;//���f�������_�[
	PhysicsStaticObject m_physicsStaticObject;//�����蔻��
};

