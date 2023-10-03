#include "stdafx.h"
#include "Game.h"
///////////////////////////////////////////////////////////
#include "P_main_Player.h"
#include "E_001_enemy.h"
#include "Map_home1.h"
#include "Ca_maincamera.h"
///////////////////////////////////////////////////////////
//マップとステージ類のヘッダーとプログラムはMap_~のプログラム。
//プレイヤーのヘッダーとプログラムはP_~のプログラム。
//カメラのヘッダーとプログラムはCa_~のプログラム。
// エネミーのヘッダーとプログラムはE_~のプログラム。
///////////////////////////////////////////////////////////

bool Game::Start()
{	
	g_camera3D->SetPosition({ 0.0f, 100.0f, -600.0f });
	g_camera3D->SetFar(40000.0f);
	//カメラ作成
	m_camera = NewGO<Ca_maincamera>(0, "camera");
	m_levelrender.Init("Assets/modelData/A_leveltest/testlevel1.tkl",[&](LevelObjectData& objData) 
	{
		//現在名testPlayer
		//プレイヤー
		if (objData.EqualObjectName(L"testPlayer") == true)
		{
			m_player = NewGO<P_main_Player>(0, "player");
			m_player->Setposition(objData.position);
			m_player->Setrotarion(objData.rotation);
			m_player->Setscale(objData.scale);
			return true;
		}
		//現在名testmap
		//最初のマップ
		else if (objData.EqualObjectName(L"testmapV1") == true)
		{
			m_home1 = NewGO<Map_home1>(0, "home1");
			m_home1->Setposition(objData.position);
			m_home1->Setrotarion(objData.rotation);
			m_home1->Setscale(objData.scale);
			return true;
		}
		//現在名enemy_001
		//敵１
		else if (objData.ForwardMatchName(L"enemy_001bone") == true)
		{
			E_001_enemy* enemy_001 = NewGO<E_001_enemy>(0, "enemy_001");
			enemy_001->Setposition(objData.position);
			enemy_001->Setrotation(objData.rotation);
			enemy_001->SetHP(5);
			m_numenemy++;
			return true;
		}
		return true;
	});	
	return true;
}

void Game::Update()
{
}

void Game::Render(RenderContext& rc)
{
}
