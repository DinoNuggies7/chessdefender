#include "player.h"

// ==================================
//  		Public Functions
// ==================================

Player::Player() {
	this->team = "Player";
	this->x = 5;
	this->y = 8;
}

// ===================================
//  		Private Functions
// ===================================

void Player::movement(float _dt) {	
	if (this->kUpP) {
		this->dir = 0;
		//this->y--;
		this->doStep = true;
	}
	else if (this->kDownP) {
		this->dir = 1;
		//this->y++;
		this->doStep = true;
	}
	else if (this->kLeftP) {
		this->dir = 2;
		//this->x--;
		this->doStep = true;
	}
	else if (this->kRightP) {
		this->dir = 3;
		//this->x++;
		this->doStep = true;
	}
}

void Player::initSprite() {
	this->image.loadFromFile("assets/pieces.png");
	this->image.createMaskFromColor(sf::Color(0x6f6d51ff));
	this->image.createMaskFromColor(sf::Color(0xf9303dff));
	this->texture.loadFromImage(this->image);
	this->sprite.setTexture(this->texture);
}

void Player::setAnimation(const char* _animation) {
	frame = 0;
	this->clipTexture(85, 61, 12, 28, false);
}