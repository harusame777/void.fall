#include "stdafx.h"
#include "Title.h"
#include "Game.h"

bool Title::Start()
{
	m_spriteRender.Init("Assets/modelData/A_screen/title.DDS", 1920.0f, 1080.0f);
	return true;
}

void Title::Update()
{
	if (g_pad[0]->IsTrigger(enButtonA)){
		NewGO<Game>(0, "game");
		DeleteGO(this);
		auto list = GameObjectManager::GetInstance()->GetGOList();
	}
	m_spriteRender.Update();
}

void Title::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
}