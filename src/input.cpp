#include "input.h"

void Input::input() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		this->kUp = true;
	else
		this->kUp = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		this->kDown = true;
	else
		this->kDown = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		this->kLeft = true;
	else
		this->kLeft = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		this->kRight = true;
	else
		this->kRight = false;

	if (this->kUp or this->kDown or this->kLeft or this->kRight)
		this->kInput = true;
	else
		this->kInput = false;

	if (this->kUp)
		this->dir = 0;
	else if (this->kUp and this->kRight)
		this->dir = 1;
	else if (this->kRight)
		this->dir = 2;
	else if (this->kRight and this->kDown)
		this->dir = 3;
	else if (this->kDown)
		this->dir = 4;
	else if (this->kDown and this->kLeft)
		this->dir = 5;
	else if (this->kLeft)
		this->dir = 6;
	else if (this->kLeft and this->kUp)
		this->dir = 7;
}