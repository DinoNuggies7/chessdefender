#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <STP/TMXLoader.hpp>

#include "globals.h"
#include "player.h"
#include "enemy.h"

class Game {
public:
	Game();
	~Game();

	bool running();

	void update();
	void render();
private:
	void initWindow();
	void initEntities();
	void updateDelta();
	void pollEvents();

	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::View view;
	sf::Event ev;
	sf::Clock clock;

	float delta, dt;
	int step;

	Player player;
	Enemy enemy;
};

#endif