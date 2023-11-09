#include "stdafx.h"
#include "Game.h"
///////////////////////////////////////////////////////////
#include "P_main_Player.h"
#include "E_001_enemy.h"
#include "E_002_enemy.h"
#include "Map_home1.h"
#include "Ca_maincamera.h"
#include "IEnemy.h"
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
		if (objData.EqualObjectName(L"voidPlayer") == true)
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
		//現在名
		//敵１
		else if (objData.ForwardMatchName(L"RE_enemy_001") == true)
		{
			E_001_enemy* enemy_001 = NewGO<E_001_enemy>(0, "enemy");
			enemy_001->Setposition(objData.position);
			enemy_001->Setrotation(objData.rotation);
			enemy_001->Setscale(objData.scale);
			enemy_001->SetHP(5);
			m_numenemy++;
			return true;
		}
		else if (objData.ForwardMatchName(L"RE_enemy_002") == true)
		{
			E_002_enemy* enemy_002 = NewGO<E_002_enemy>(0, "enemy_002");
			enemy_002->Setposition(objData.position);
			enemy_002->Setrotation(objData.rotation);
			enemy_002->Setscale(objData.scale);
			enemy_002->SetHP(5);
			m_numenemy++;
			return true;
		}
		return true;
	});	
	return true;
}

void Game::Update()
{
	wchar_t wcsbuf[256];

	swprintf_s(wcsbuf, 256, L"HP:%01d||MP:%01d", int(m_player->m_hp)
	,int(m_player->m_mp));

	//表示するテキストを設定。
	m_fontrender.SetText(wcsbuf);
	//フォントの位置を設定。
	m_fontrender.SetPosition(Vector3(450.0f, 450.0f, 0.0f));
	//フォントの大きさを設定。
	m_fontrender.SetScale(1.0f);
	//フォントの色を設定。
	m_fontrender.SetColor({ 0.0f,0.0f,0.0f,1.0f });

}

void Game::Render(RenderContext& rc)
{
	m_fontrender.Draw(rc);
}

