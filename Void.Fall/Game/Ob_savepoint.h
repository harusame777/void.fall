#pragma once
class P_main_Player;
class Game;
class Ob_savepoint : public IGameObject
{
public:
	bool Start();
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
	void SetNum(int num)
	{
		savenum = num;
	}
	void SetsaveVec()
	{
		Vector3 correpos{ 0.0,0.0,-60.0 };
		m_Saveposition = m_position + correpos;
	}
	Vector3 m_position;						//���W
	Quaternion m_rotation;					//��]
	Vector3 m_scale = Vector3::One;			//�傫��
	CollisionObject* m_collisionObject;		//�R���W�����I�u�W�F�N�g�B
	Vector3	m_forward = Vector3::AxisZ;		//���g�̐��ʃx�N�g���B
	P_main_Player* m_player = nullptr;		//�v���C���[
	Game* m_game = nullptr;                 //�Q�[��
	ModelRender m_modelrender;//���f�������_�[
	PhysicsStaticObject m_physicsStaticObject;//�����蔻��
	Vector3 m_Saveposition;						//�Z�[�u�|�W�V����
	int savenum = 0;
};

