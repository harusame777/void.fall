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
//アニメーション類/////////////////////////////////////////
	//アニメーションステート
	enum EnAnimationClip {
		enAnimationClip_Move,//bullet移動
		enAnimationClip_Num//アニメーション数
	};
///////////////////////////////////////////////////////////
//初期設定系統
//メンバ関数宣言
	AnimationClip m_animationclips[enAnimationClip_Num];     //アニメーションクリップ
	ModelRender* m_modelrender = nullptr;								//モデルレンダー

};

