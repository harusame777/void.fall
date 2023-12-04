#include "stdafx.h"
#include "mapMaker.h"
#include "Imap.h"
#include <time.h>
#include <stdlib.h>

namespace {
	const int BranchNone = 0;
}

bool mapMaker::Start()
{
	m_map = FindGO<Imap>("map");
	//乱数を初期化。
	srand((unsigned)time(NULL));
	return true;
}

void mapMaker::Update()
{
	MapVecMaker();
}

void mapMaker::MapVecMaker()
{
	switch (m_makeProcess)
	{
	case mapMaker::en_start_make:
		StartMake();
		break;
	case mapMaker::en_route_make:
		RouteMake();
		break;
	case mapMaker::en_goal_make:
		break;
	case mapMaker::en_branch_make:
		break;
	case mapMaker::en_branchend_make:
		break;
	}
}

void mapMaker::StartMake()
{
	//ここにスタート地点のモデルを入れる
	/*                                */
	////////////////////////////////////
	//ここら辺は位置調整
	/*                */
	////////////////////
	//2次元配列の0,0がスタート地点とする。
	//MapVec_List.at(BranchNone).push_back(ここにImapのスタート地点のデータが入る);
	m_mapPartsnum++;
	m_makeProcess = en_route_make;
}

void mapMaker::RouteMake()
{

}
