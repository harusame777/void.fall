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
	//�������������B
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
	//�����ɃX�^�[�g�n�_�̃��f��������
	/*                                */
	////////////////////////////////////
	//������ӂ͈ʒu����
	/*                */
	////////////////////
	//2�����z���0,0���X�^�[�g�n�_�Ƃ���B
	//MapVec_List.at(BranchNone).push_back(������Imap�̃X�^�[�g�n�_�̃f�[�^������);
	m_mapPartsnum++;
	m_makeProcess = en_route_make;
}

void mapMaker::RouteMake()
{

}
