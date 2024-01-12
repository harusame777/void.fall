#pragma once
class Game;
class Title;
class GameClear : public IGameObject
{
public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	Game* m_game = nullptr;
	Title* m_title = nullptr;
	SpriteRender m_spriteRender;
	SoundSource* gameBGM = nullptr;		//ÉQÅ[ÉÄíÜÇÃBGMÅB
};

