#ifndef LEVEL_H
#define LEVEL_H

#include "SFMLOrthogonalLayer.hpp"
#include "player.h"
#include "enemy.h"

#include <SFML/Graphics.hpp>
#include <tinyxml2.h>

class Level {
public:
	void init();
	void render(sf::RenderWindow* _window, int _layer);

	int entities, playerID;
	std::vector<Entity*> entity;
	std::vector<std::vector<int>> mapLayerCollision;
private:
	void initEntities();

	tmx::Map map;
	MapLayer mapLayerFloor;
	MapLayer mapLayerWalls1;
	MapLayer mapLayerWalls2;
	MapLayer mapLayerCeiling;
};

#endif