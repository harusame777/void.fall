#include "stdafx.h"
#include "Title.h"
#include "Game.h"

bool Title::Start()
{
	m_spriteRender.Init("Assets/modelData/A_screen/title.DDS", 1920.0f, 1080.0f);
	g_soundEngine->ResistWaveFileBank(0, "Assets/sound/bgmtitle.wav");//title
	g_soundEngine->ResistWaveFileBank(1, "Assets/sound/dec.wav");//title
	gameBGM = NewGO<SoundSource>(0);
	gameBGM->Init(0);
	gameBGM->Play(true);
	return true;
}

void Title::Update()
{
	if (g_pad[0]->IsTrigger(enButtonA)){
		SoundSource* se = NewGO<SoundSource>(1);
		se = NewGO<SoundSource>(1);
		se->Init(1);
		se->Play(false);
		NewGO<Game>(0, "game");
		DeleteGO(gameBGM);
		DeleteGO(this);
		auto list = GameObjectManager::GetInstance()->GetGOList();
	}
	m_spriteRender.Update();
}

void Title::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
}