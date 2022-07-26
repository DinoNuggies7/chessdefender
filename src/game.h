#ifndef GAME_H
#define GAME_H

#include "globals.h"
#include "player.h"
#include "enemy.h"
#include "SFMLOrthogonalLayer.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <tinyxml2.h>

#include <iostream>
#include <random>
#include <vector>
#include <string>
#include <sstream>

class Game {
public:
	Game();
	~Game();

	bool running();

	void update();
	void render();
private:
	void initEntities();
	void initWindow();
	void updateDelta();
	void pollEvents();

	void print(std::string _string, int _x = 0, int _y = 0, int _size = 24, sf::Color _color = sf::Color::Black);
	std::vector<sf::Text> text;

	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::View view;
	sf::Event ev;
	sf::Clock clock;
	sf::Font font;

	tmx::Map map;
	MapLayer mapLayerFloor;
	MapLayer mapLayerWall;
	MapLayer mapLayerCeiling;
	std::stringstream value;
	std::vector<std::vector<int>> mapLayerCollision;
	std::vector<std::vector<int>> _mapLayerCollision {
		{79,454,454,454,454,454,454,454,454,454,454,454,454,454,454,454,454,454,454,454,454,454,454,455,70,},
		{274,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,259,},
		{274,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,259,},
		{274,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,259,},
		{274,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,195,210,0,0,0,0,0,259,},
		{274,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,259,274,0,0,0,0,0,259,},
		{274,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,323,338,0,0,0,0,0,259,},
		{274,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,387,402,0,0,0,0,0,259,},
		{274,0,0,346,0,0,0,350,0,0,0,0,0,0,0,0,0,451,466,0,0,0,0,0,259,},
		{274,0,0,410,0,0,0,414,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,259,},
		{274,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,259,},
		{338,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,259,},
		{402,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,259,},
		{338,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,195,210,0,0,0,0,0,0,259,},
		{402,0,0,0,0,195,198,199,200,210,0,0,0,0,0,0,451,466,0,0,0,0,0,0,259,},
		{274,0,0,0,0,387,264,264,264,402,0,0,0,0,0,0,0,0,0,0,0,0,0,0,259,},
		{274,0,0,0,0,451,455,456,457,466,0,0,0,0,0,0,0,0,0,0,0,0,0,0,259,},
		{274,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,259,},
		{212,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,216,},
		{73,198,198,198,198,198,198,198,198,198,198,198,198,198,198,198,198,198,199,200,201,199,200,201,76}
	};

	float delta, dt;
	bool gameOver;
	int step, entities, entityCounter;
	int playerID;
	std::vector<Entity*> entity;
};

#endif