#include "input.h"

// =====================
//  Protected Functions
// =====================

void Input::input() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) and !this->kUp)
		this->kUpP = true;
	else
		this->kUpP = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) and !this->kDown)
		this->kDownP = true;
	else
		this->kDownP = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) and !this->kLeft)
		this->kLeftP = true;
	else
		this->kLeftP = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) and !this->kRight)
		this->kRightP = true;
	else
		this->kRightP = false;

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
}