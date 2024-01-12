#pragma once
class Game;
class Title : public IGameObject
{
public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	SpriteRender m_spriteRender;
	Game* m_game = nullptr;
	SoundSource* gameBGM = nullptr;		//ÉQÅ[ÉÄíÜÇÃBGMÅB
};

