#ifndef INPUT_H
#define INPUT_H

#include "globals.h"

#include <SFML/Graphics.hpp>

class Input {
public:
	int fullscreen;
protected:
	void input(sf::RenderWindow& _window);

	bool kInput, kUp, kDown, kLeft, kRight, kUpP, kDownP, kLeftP, kRightP, kA, kB, kMouseL, kMouseR, kMouseLP, kMouseRP;
	int mousePosMod;
	sf::Vector2i kMouse;
};

#endif