#include "stdafx.h"
#include "GameClear.h"
#include "Game.h"
#include "Title.h"

bool GameClear::Start()
{
	m_spriteRender.Init("Assets/modelData/A_screen/gameclear.DDS", 1920.0f, 1080.0f);
	m_game = FindGO<Game>("game");
	m_game->Deletegame();
	g_soundEngine->ResistWaveFileBank(3, "Assets/sound/bgmgameclear.wav");
	gameBGM = NewGO<SoundSource>(3);
	gameBGM->Init(3);
	gameBGM->Play(true);
	return true;
}

void GameClear::Update()
{
	if (g_pad[0]->IsTrigger(enButtonA)) {
		SoundSource* se = NewGO<SoundSource>(1);
		se = NewGO<SoundSource>(1);
		se->Init(1);
		se->Play(false);
		NewGO<Title>(0,"title");
		DeleteGO(gameBGM);
		DeleteGO(this);
	}
	//ï`âÊèàóù
	m_spriteRender.Update();
}

void GameClear::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
}