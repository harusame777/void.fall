#pragma once
#include "Imap.h"
#include "M_parts_wall.h"
class Imap;
class M_parts_wall;
class mapMaker : public IGameObject
{
public:
	bool Start();
	void mapdatareset();
	void makespace();
	bool makeroomdata(int roomheight,int roomwidth,int roompointX,int roompointY);
	void makeroaddata(int roadStarpointX, int roadStartpointY, int meetpointX, int meetpointY);
	void makewall();
	int *map;
	M_parts_wall* map_wall = nullptr;
	
};

