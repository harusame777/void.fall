#pragma once
class P_main_Player;
class Game;
class Ob_savepoint : public IGameObject
{
public:
	enum State {
		en_Standby,
		en_save,
	};
	enum EnAnimationClip {
		enAnimationClip_Standby,//�_����
		enAnimationClip_Save,
		enAnimationClip_Num,
	};
	bool Start();
	void Render(RenderContext& rc);
	void Update();
	void Collision();										//�{�̂̓����蔻��
	void PlayAnimation();
	void ManageState();//�J�ڏ���
	void ProcessSaveStateTransition();//�Z�[�u�J��
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
		Vector3 correpos{ 0.0,0.0,-150.0 };
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
	AnimationClip m_animationclips[enAnimationClip_Num];     //�A�j���[�V�����N���b�v
	State m_state = en_Standby;
	int savenum = 0;
};

