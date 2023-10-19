#pragma once
class P_main_Player;
class B_homingbullet : public IGameObject
{
public:
///////////////////////////////////////////////////////////
	B_homingbullet() {};
	~B_homingbullet() {};
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void Movebullet();
	void Inpact();
	void CalcVelocity(const float speed, const float curvatureRadius,
		const float damping);
///////////////////////////////////////////////////////////
//�A�j���[�V������/////////////////////////////////////////
	//�A�j���[�V�����X�e�[�g
	enum EnAnimationClip {
		enAnimationClip_Move,//bullet�ړ�
		enAnimationClip_Num//�A�j���[�V������
	};
//�A�j���[�V�����֐�///////////////////////////////////////
	void PlayAnimation();
///////////////////////////////////////////////////////////
//�����ݒ�n��
	void SetVelocity(const Vector3& velocity)
	{
		m_velocity = velocity;
	}
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	void SetTarget(const Vector3& targetPosition)
	{
		m_targetPosition = targetPosition;
	}
//�����o�֐��錾
	float bullettime = 10.0f;
	CollisionObject* m_collisionObject;						//�R���W�����I�u�W�F�N�g�B
	P_main_Player* m_player = nullptr;                      //�v���C���[
	AnimationClip m_animationclips[enAnimationClip_Num];    //�A�j���[�V�����N���b�v
	ModelRender* m_modelrender = nullptr;					//���f�������_�[
	Vector3 m_position;										//���W
	Vector3 m_velocity;										//���x
	Vector3 m_targetPosition;								//�ǔ��Ώۍ��W


};

