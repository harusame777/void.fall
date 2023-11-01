#pragma once
#include "IEnemy.h"
class E_002_enemy : public IEnemy
{
public:
	//�A�j���[�V������/////////////////////////////////////////
//�A�j���[�V�����X�e�[�g
	enum EnAnimationClip {
		enAnimationClip_Idle,//�_����
		enAnimationClip_Num//�A�j���[�V������
	};
	E_002_enemy() {};
	~E_002_enemy() {};
	bool Start();
	void Update();                                         //�A�b�v�f�[�g
	void Render(RenderContext& rc);                        //���f�������_�[
	///////////////////////////////////////////////////////////
	//�����ݒ�n��
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
	//void SetALLposition()
	//{
	//	m_enemyList[m_game->m_numenemy] = m_enemy002;
	//}
	//�����o�֐��錾
	E_002_enemy* m_enemy002 = nullptr;
	AnimationClip m_animationclips[enAnimationClip_Num];     //�A�j���[�V�����N���b�v
	ModelRender* m_modelrender = nullptr;					//���f�������_�[
	Vector3 m_position;										//���W
	Vector3 m_scale = Vector3::One;							//�傫��
	Quaternion m_rotation;									//��]
	CharacterController	m_charaCon;							//�L�����N�^�[�R���g���[���[�B
	int m_hp = 0;                                           //HP
};

