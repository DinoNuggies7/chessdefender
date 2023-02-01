#ifndef LEVEL_H
#define LEVEL_H

#include "SFMLOrthogonalLayer.hpp"
#include "player.h"
#include "enemy.h"
#include "door.h"

#include <SFML/Graphics.hpp>
#include <tinyxml2.h>
#include <vector>
#include <random>

class Level {
public:
	void init(int _levelID, int _door0 = true, int _door1 = true, int _door2 = true, int _door3 = true);
	void render(sf::RenderWindow& _window, int _layer);
	void initDoors(int _door0 = true, int _door1 = true, int _door2 = true, int _door3 = true);
	void randomizeRoomGen(int& _warp0, int& _warp1, int& _warp2);
	void generateRoom(int _warp0, int _warp1, int _warp2, int _warp3, int _clear);

	int playerID, currentRoom;
	std::vector<Entity*> entity;
	std::vector<int> room[5];
	std::vector<std::vector<int>> mapLayerCollision;
private:
	void initCollision();
	void initEntities();

	tmx::Map map;
	MapLayer mapLayerFloor;
	MapLayer mapLayerWalls1;
	MapLayer mapLayerWalls2;
	MapLayer mapLayerCeiling;
};

#endif