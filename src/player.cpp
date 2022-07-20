#include "player.h"

// Protected Functions
void Player::movement(float _dt) {	
	if (this->kUpP)
		this->y -= 16;
	else if (this->kDownP)
		this->y += 16;
	else if (this->kLeftP)
		this->x -= 16;
	else if (this->kRightP)
		this->x += 16;
}

void Player::initSprite() {
	this->image.loadFromFile("assets/pieces.png");
	this->image.createMaskFromColor(sf::Color(0x6f6d51ff));
	this->image.createMaskFromColor(sf::Color(0xf9303dff));
	this->texture.loadFromImage(this->image);
	this->sprite.setTexture(this->texture);
}