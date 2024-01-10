#include "stdafx.h"
#include "GameClear.h"
#include "Game.h"
#include "Title.h"

bool GameClear::Start()
{
	m_spriteRender.Init("Assets/modelData/A_screen/gameclear.DDS", 1920.0f, 1080.0f);
	m_game = FindGO<Game>("game");
	DeleteGO(m_game);
	return true;
}

void GameClear::Update()
{
	if (g_pad[0]->IsTrigger(enButtonA)) {
		NewGO<Title>(0,"title");
		DeleteGO(this);
	}
	//•`‰æˆ—
	m_spriteRender.Update();
}

void GameClear::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
}