#pragma once
class B_homingbullet : public IGameObject
{
public:
///////////////////////////////////////////////////////////
	B_homingbullet() {};
	~B_homingbullet() {};
	bool Start();
	void Update();
	void Render(RenderContext& rc);
///////////////////////////////////////////////////////////
//�A�j���[�V������/////////////////////////////////////////
	//�A�j���[�V�����X�e�[�g
	enum EnAnimationClip {
		enAnimationClip_Move,//bullet�ړ�
		enAnimationClip_Num//�A�j���[�V������
	};
///////////////////////////////////////////////////////////
//�����ݒ�n��
//�����o�֐��錾
	AnimationClip m_animationclips[enAnimationClip_Num];     //�A�j���[�V�����N���b�v
	ModelRender* m_modelrender = nullptr;								//���f�������_�[

};

