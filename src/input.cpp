#include "input.h"

// ==================================
//  		Public Functions
// ==================================

Input::Input() {
	if (!Global::START_FULLSCREEN)
		this->fullscreen = Global::WIN_SCALE;
}


// =====================================
//  		Protected Functions
// =====================================

// For getting Mouse and Keyboard Input
void Input::input(sf::RenderWindow& _window, bool& _focused) {
	if (_focused) {
		if (this->fullscreen != 0)
			this->mousePosMod = 16 * this->fullscreen;
		else
			this->mousePosMod = 16 * Global::WIN_SCALE;

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) and !this->kMouseL)
			this->kMouseLP = true;
		else
			this->kMouseLP = false;

		if (sf::Mouse::isButtonPressed(sf::Mouse::Right) and !this->kMouseR)
			this->kMouseRP = true;
		else
			this->kMouseRP = false;

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			this->kMouseL = true;
		else
			this->kMouseL = false;

		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
			this->kMouseR = true;
		else
			this->kMouseR = false;

		this->kMouse = sf::Mouse::getPosition(_window);

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

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) or this->kUpP)
			this->kUp = true;
		else
			this->kUp = false;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) or this->kDownP)
			this->kDown = true;
		else
			this->kDown = false;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) or this->kLeftP)
			this->kLeft = true;
		else
			this->kLeft = false;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) or this->kRightP)
			this->kRight = true;
		else
			this->kRight = false;
	}
}