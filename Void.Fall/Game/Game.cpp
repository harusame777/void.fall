#include "stdafx.h"
#include "Game.h"
#include "Level3DRender/LevelRender.h"
#include "P_main_Player.h"
#include "E_001_enemy.h"
#include "E_002_enemy.h"
#include "E_003_enemy.h"
#include "E_004_enemy.h"
#include "Map_home1.h"
#include "Ca_maincamera.h"
#include "IEnemy.h"
#include "Imap.h"
#include "IItem.h"
#include "M_parts1.h"
#include "M_parts2.h"
#include "M_parts3.h"
#include "M_parts4.h"
#include "M_parts4_sub.h"
#include "M_parts5.h"
#include "mapMaker.h"
#include "Ob_savepoint.h"
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
//マップとステージ類のヘッダーとプログラムはMap_~のプログラム。
//プレイヤーのヘッダーとプログラムはP_~のプログラム。
//カメラのヘッダーとプログラムはCa_~のプログラム。
// エネミーのヘッダーとプログラムはE_~のプログラム。
///////////////////////////////////////////////////////////

namespace
{
	const Vector3 corre1 = { 0.0f,60.0f,0.0f };//位置修正
}

bool Game::Start()
{	
	g_camera3D->SetPosition({ 0.0f, 100.0f, -600.0f });
	g_camera3D->SetFar(40000.0f);
	//カメラ作成
	m_camera = NewGO<Ca_maincamera>(0, "camera");
	m_spriterender.Init("Assets/modelData/A_screen/Down.DDS", 1920.0f, 1080.0f);
	m_levelrender1.Init("Assets/modelData/A_leveltest/testlevel2.tkl",[&](LevelObjectData& objData)
	{
		//現在名testPlayer
		//プレイヤー
		//m_mapMaker = NewGO<mapMaker>(0, "mapmaker");
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
		else if (objData.ForwardMatchName(L"mapparts1") == true)
		{
			M_parts1* m_mapparts1 = NewGO<M_parts1>(0, "parts");
			m_mapparts1->Setposition(objData.position);
			m_mapparts1->Setrotarion(objData.rotation);
			m_mapparts1->Setscale(objData.scale);
			return true;
		}
		else if (objData.ForwardMatchName(L"mapparts2") == true)
		{
			M_parts2* m_mapparts2 = NewGO<M_parts2>(0, "parts");
			m_mapparts2->Setposition(objData.position);
			m_mapparts2->Setrotarion(objData.rotation);
			m_mapparts2->Setscale(objData.scale);
			return true;
		}
		else if (objData.ForwardMatchName(L"mapparts3") == true)
		{
			M_parts3* m_mapparts3 = NewGO<M_parts3>(0, "parts");
			m_mapparts3->Setposition(objData.position);
			m_mapparts3->Setrotarion(objData.rotation);
			m_mapparts3->Setscale(objData.scale);
			return true;
		}
		else if (objData.ForwardMatchName(L"mapparts4") == true)
		{
			M_parts4* m_mapparts4 = NewGO<M_parts4>(0, "parts");
			m_mapparts4->Setposition(objData.position);
			m_mapparts4->Setrotarion(objData.rotation);
			m_mapparts4->Setscale(objData.scale);
			m_mapparts4->Setnum(map4_num);
			m_Map4List.push_back(m_mapparts4);
			E_004_enemy* enemy_004 = NewGO<E_004_enemy>(0, "enemy_004");
			enemy_004->Setposition(objData.position + corre1);
			enemy_004->Setnum(map4_num);
			m_MapLock.push_back(enemy_004);
			map4_num++;
			GoalLockNum++;
			return true;
		}
		else if (objData.ForwardMatchName(L"mapparts5") == true)
		{
			M_parts5* m_mapparts5 = NewGO<M_parts5>(0, "parts");
			m_mapparts5->Setposition(objData.position);
			m_mapparts5->Setrotarion(objData.rotation);
			m_mapparts5->Setscale(objData.scale);
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
			enemy_001->SetHP(1);
			enemy_001->SetVectornum(m_numenemy);
			enemy_001->SetEnemyType(E_001_enemy::en_enemy001);
			m_numenemy++;
			m_EnemyList.push_back(enemy_001);
			return true;
		}
		else if (objData.ForwardMatchName(L"RE_enemy_002") == true)
		{
			E_002_enemy* enemy_002 = NewGO<E_002_enemy>(0, "enemy");
			enemy_002->Setposition(objData.position);
			enemy_002->Setrotation(objData.rotation);
			enemy_002->Setscale(objData.scale);
			enemy_002->SetHP(1);
			enemy_002->SetVectornum(m_numenemy);
			enemy_002->SetEnemyType(E_002_enemy::en_enemy002);
			m_numenemy++;
			m_EnemyList.push_back(enemy_002);
			return true;
		}
		//else if (objData.ForwardMatchName(L"RE_enemy_003") == true)
		//{
		//	E_003_enemy* enemy_003 = NewGO<E_003_enemy>(0, "enemy");
		//	enemy_003->Setposition(objData.position);
		//	enemy_003->Setrotation(objData.rotation);
		//	enemy_003->Setscale(objData.scale);
		//	enemy_003->SetHP(1);
		//	enemy_003->SetVectornum(m_numenemy);
		//	enemy_003->SetEnemyType(E_003_enemy::en_enemy003);
		//	m_numenemy++;
		//	m_EnemyList.push_back(enemy_003);
		//	return true;
		//}
		//オブジェクト
		else if (objData.ForwardMatchName(L"savepoint") == true)
		{
			Ob_savepoint* m_savepoint = NewGO<Ob_savepoint>(0, "savepoint");
			m_savepoint->Setposition(objData.position);
			m_savepoint->Setrotarion(objData.rotation);
			m_savepoint->Setscale(objData.scale);
			m_savepoint->SetNum(m_savenum);
			m_saveList.push_back(m_savepoint);
			m_savenum++;
			return true;
		}
		return true;
	});	
	auto list = GameObjectManager::GetInstance()->GetGOList();

	return true;
}

void Game::Update()
{
	auto list = GameObjectManager::GetInstance()->GetGOList();
	Font();
	Save();
	Down();
	m_spriterender.Update();
}

void Game::Font()
{
	wchar_t wcsbuf[256];

	swprintf_s(wcsbuf, 256, L"HP:%01d||MP:%01d", int(m_player->m_hp)
		, int(m_player->m_mp));

	//表示するテキストを設定。
	m_fontrender.SetText(wcsbuf);
	//フォントの位置を設定。
	m_fontrender.SetPosition(Vector3(450.0f, 450.0f, 0.0f));
	//フォントの大きさを設定。
	m_fontrender.SetScale(1.0f);
	//フォントの色を設定。
	m_fontrender.SetColor({ 0.0f,0.0f,0.0f,1.0f });
}

void Game::Save()
{
	Vector3 m_NowPlayerpos = m_player->m_position;
	if (m_NowPlayerpos.y < Recoverypos.y){
		if (m_Nowsavepointnum == -1){
			Vector3 pos = { 0.0 ,60.0,0.0 };
			m_player->m_position = pos;
			m_player->m_charaCon.SetPosition(pos);
		}
		else{
			m_player->m_position = m_saveList[m_Nowsavepointnum]->m_Saveposition;
			m_player->m_charaCon.SetPosition(m_saveList[m_Nowsavepointnum]->m_Saveposition);
		}
	}
}

void Game::Down()
{
	if (m_playstate == en_down){
		if (g_pad[0]->IsTrigger(enButtonB)) {
			RelocationEnemy();
			RelocationEnemy004();
			if (m_Nowsavepointnum == -1) {
				Vector3 pos = { 0.0 ,60.0,0.0 };
				m_player->m_position = pos;
				m_player->m_charaCon.SetPosition(pos);
			}
			else {
				m_player->m_position = m_saveList[m_Nowsavepointnum]->m_Saveposition;
				m_player->m_charaCon.SetPosition(m_saveList[m_Nowsavepointnum]->m_Saveposition);
			}
			m_player->RelocationPlayer();
			m_playstate = en_play;
		}
	}
}

bool Game::RelocationEnemy()
{
	m_player = FindGO<P_main_Player>("player");
	m_player->m_LockonTF = false;
	m_EnemyList.clear();
	QueryGOs<IEnemy>("enemy", [&](IEnemy* ienemy) {
		DeleteGO(ienemy);
		return true;
	});
	m_numenemy = 0;
	m_player->m_LockonTF = true;
	m_levelrender2.Init("Assets/modelData/A_leveltest/testlevel2.tkl", [&](LevelObjectData& objData)
	{
		if (objData.ForwardMatchName(L"RE_enemy_001") == true)
		{
			E_001_enemy* enemy_001 = NewGO<E_001_enemy>(0, "enemy");
			enemy_001->Setposition(objData.position);
			enemy_001->Setrotation(objData.rotation);
			enemy_001->Setscale(objData.scale);
			enemy_001->SetHP(1);
			enemy_001->SetVectornum(m_numenemy);
	     	enemy_001->SetEnemyType(E_001_enemy::en_enemy001);
			m_numenemy++;
			m_EnemyList.push_back(enemy_001);
			return true;
		}
		else if (objData.ForwardMatchName(L"RE_enemy_002") == true)
		{
			E_002_enemy* enemy_002 = NewGO<E_002_enemy>(0, "enemy");
			enemy_002->Setposition(objData.position);
			enemy_002->Setrotation(objData.rotation);
			enemy_002->Setscale(objData.scale);
			enemy_002->SetHP(1);
			enemy_002->SetVectornum(m_numenemy);
			enemy_002->SetEnemyType(E_002_enemy::en_enemy002);
			m_numenemy++;
			m_EnemyList.push_back(enemy_002);
			return true;
		}
	});
	return true;
}

void  Game::RelocationEnemy004()
{
	QueryGOs<E_004_enemy>("enemy_004", [&](E_004_enemy* enemy004) {
		if (enemy004->m_enemystate ==
			enemy004->enAnimationClip_Active){
			enemy004->Standby();
		}
		return true;
	});
	m_mapparts4_sub = FindGO<M_parts4_sub>("parts4_sub");
	if (m_mapparts4_sub != nullptr) {
		m_mapparts4_sub->map_delete = true;
	}
	SummonEnemynum = 0;
}

void Game::Delete_EnemyVec(const int num)
{
	m_EnemyList.erase(m_EnemyList.begin() + num);
	for (int VecNow = num; VecNow < m_EnemyList.size(); VecNow++){
		m_EnemyList[VecNow]->m_Vectornum -= 1;
	}
	m_player->m_LockonTF = true;
}

void Game::Render(RenderContext& rc)
{
	m_fontrender.Draw(rc);
	if (m_playstate == en_down) {
		m_spriterender.Draw(rc);
	}
}

Game::~Game()
{
	DeleteGO(m_camera);
	DeleteGO(m_player);
	DeleteMap();
	Deletesavepoint();
	DeleteEnemy();
	DeleteItem();
}

void Game::DeleteMap()
{
	QueryGOs<Imap>("parts", [&](Imap* maps) {
		DeleteGO(maps);
		return true;
	});
	DeleteGO(m_mapparts5);
}

void Game::DeleteEnemy()
{
	QueryGOs<IEnemy>("enemy", [&](IEnemy* ienemy) {
		DeleteGO(ienemy);
		return true;
	});
	QueryGOs<E_004_enemy>("enemy_004", [&](E_004_enemy* enemy004) {
		DeleteGO(m_enemy_004);
		return true;
	});
}

void Game::DeleteItem()
{
	QueryGOs<IItem>("item", [&](IItem* item) {
		DeleteGO(item);
		return true;
	});
}

void Game::Deletesavepoint()
{
	QueryGOs<Ob_savepoint>("savepoint", [&](Ob_savepoint* savepoint){
		DeleteGO(savepoint);
		return true;
	});
}