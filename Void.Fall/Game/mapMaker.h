#pragma once
class Imap;
class mapMaker : public IGameObject
{
public:
	enum MapMakeProcess{
		en_start_make,
		en_route_make,
		en_goal_make,
		en_branch_make,
		en_branchend_make,
	};
public:
	bool Start();
	void Update();
	void MapVecMaker();
	void StartMake();
	void RouteMake();
public:
	Imap* m_map = nullptr;
	std::vector< std::vector<Imap*> > MapVec_List;
	MapMakeProcess m_makeProcess = en_start_make;
	bool m_goalbool = false;
	int m_mapPartsnum = 0;
	int m_mapBranchnum = 0;
	int m_mapBranchVecnum = 0;
};

