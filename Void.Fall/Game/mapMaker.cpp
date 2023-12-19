#include "stdafx.h"
#include "mapMaker.h"
#include <time.h>
#include <stdlib.h>

namespace {
	const int Mapwidth = 50;
	const int Mapheight = 50;

	const int roomMinheight = 3;
	const int roomMaxheight = 5;

	const int roomMinwidth = 3;
	const int roomMaxwidth = 5;

	const int roomcountMin = 3;
	const int roomcountMax = 5;

	const int road = 0;
	const int wall = 9;

	const int meetpointscount = 1;
}

bool mapMaker::Start()
{
	srand((unsigned)time(NULL));
	mapdatareset();
	makespace();
	makewall();
	return true;
}

void mapMaker::mapdatareset()
{
	map = new int[Mapheight, Mapwidth];
	for (int i = 0; i < Mapheight; i++){
		for (int j = 0; j < Mapwidth; j++){
			map[i, j] = wall;
		}
	}
}

void mapMaker::makespace()
{
	int roomCount = rand() % roomcountMax + roomcountMin;
	int meetpointsX[meetpointscount];
	int meetpointsY[meetpointscount];
	for (int i = 0; i < meetpointscount; i++){
		meetpointsX[i] = rand() % Mapwidth / 4 + Mapwidth * 3 / 4;
		meetpointsY[i] = rand() % Mapheight / 4 + Mapheight * 3 / 4;
		map[meetpointsY[i], meetpointsX[i]] = road;
	}
	for (int i = 0; i < roomCount; i++){
		int roomheight = rand() % roomMaxheight + roomMinheight;
		int roomwidth = rand() % roomMaxheight + roomMaxwidth;
		int roompointX = rand() % 2 + 2 - Mapwidth - roomMaxwidth;
		int roompointY = rand() % 2 + 2 - Mapheight - roomMaxheight;

		int roadStartpointX = rand() % roompointX + roompointX + roomwidth;
		int roadStartpointY = rand() % roompointY + roompointY + roomheight;

		bool isRoad = makeroomdata(roomheight, roomwidth, roompointX, roompointY);

		if (isRoad == false) {
			makeroaddata(roadStartpointX, roadStartpointY
				, meetpointsX[rand() % 0 + 0], meetpointsY[rand() % 0 + 0]);
		}
 	}
}

bool mapMaker::makeroomdata(
	int roomheight, int roomwidth, int roompointX, int roompointY)
{
	bool bemakeroad = false;
	for (int i = 0; i < roomheight; i++){
		for (int j = 0; j < roomwidth; j++){
			if (map[roompointY + 1, roompointX + 1] == road) {
				bemakeroad = true;
			}
			else {
				map[roompointY + 1, roompointX + 1] = road;
			}
		}
	}
	return bemakeroad;
}

void mapMaker::makeroaddata(
	int roadStarpointX, int roadStartpointY, int meetpointX, int meetpointY)
{
	bool isRight;
	if (roadStarpointX > meetpointX) {
		isRight = true;
	}
	else{
		isRight = false;
	}
	bool isUnder;
	if (roadStartpointY > meetpointY) {
		isUnder = false;
	}
	else{
		isUnder = true;
	}

	if (0 == rand() % 2) {

		while (roadStarpointX != meetpointX){

			map[roadStartpointY, roadStarpointX] = road;
			if (isRight == true){
				roadStarpointX--;
			}
			else{
				roadStarpointX++;
			}

		}
		while (roadStartpointY != meetpointY){
			map[roadStartpointY, roadStarpointX] = road;
			if (isUnder == true){
				roadStartpointY++;
			}
			else{
				roadStartpointY--;
			}
		}

	}
	else{
		while (roadStartpointY != meetpointY){
			map[roadStartpointY, roadStarpointX] = road;
			if (isUnder == true){
				roadStartpointY++;
			}
			else{
				roadStartpointY--;
			}
		}

		while (roadStarpointX != meetpointX){
			map[roadStartpointY, roadStarpointX] = road;
			if (isRight == true){
				roadStarpointX--;
			}
			else{
				roadStarpointX++;
			}

		}

	}
}

void mapMaker::makewall()
{
	Vector3 wallvecXY = { 0.0f, 0.0f, 0.0f };
	for (int i = 0; i < Mapheight; i++) {
		if (i > 0){
			wallvecXY.z += -500;
		}
		for (int j = 0; j < Mapwidth; j++) {
			if (map[i, j] == wall) {
				M_parts_wall* m_wall = NewGO<M_parts_wall>(0, "walls");
				m_wall->Setposition(wallvecXY);
			}
			wallvecXY.x += 500.0f;
		}
		wallvecXY.x = 0.0f;
	}
}
