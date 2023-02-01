#ifndef INPUT_H
#define INPUT_H

#include "globals.h"

#include <SFML/Graphics.hpp>

class Input {
public:
	Input();

	float fullscreen;
protected:
	void input(sf::RenderWindow& _window, bool& _focused);

	bool kUp, kDown, kLeft, kRight, kUpP, kDownP, kLeftP, kRightP, kA, kB, kMouseL, kMouseR, kMouseLP, kMouseRP;
	float mousePosMod;
	sf::Vector2i kMouse;
};

#endif