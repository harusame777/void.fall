#pragma once
class Map_home1 : public IGameObject
{
public:
	//�֐��錾
	Map_home1(){}
	~Map_home1(){}
	bool Start();
	void Update();
	void Render(RenderContext& rc);
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

