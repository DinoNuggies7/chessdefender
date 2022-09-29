#ifndef INPUT_H
#define INPUT_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class Input {
protected:
	void input(sf::RenderWindow& _window);

	bool kInput, kUp, kDown, kLeft, kRight, kUpP, kDownP, kLeftP, kRightP, kA, kB, kMouseL, kMouseR;
	sf::Vector2i kMouse;
};

#endif