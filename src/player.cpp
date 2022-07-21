#include "player.h"

// Protected Functions
void Player::movement(float _dt) {	
	if (this->kUpP) {
		this->y -= 16;
		this->doStep = true;
	}
	else if (this->kDownP) {
		this->y += 16;
		this->doStep = true;
	}
	else if (this->kLeftP) {
		this->x -= 16;
		this->doStep = true;
	}
	else if (this->kRightP) {
		this->x += 16;
		this->doStep = true;
	}
}

void Player::initSprite() {
	this->image.loadFromFile("assets/pieces.png");
	this->image.createMaskFromColor(sf::Color(0x6f6d51ff));
	this->image.createMaskFromColor(sf::Color(0xf9303dff));
	this->texture.loadFromImage(this->image);
	this->sprite.setTexture(this->texture);

	this->initiative = 1;
}

void Player::setAnimation(const char* _animation) {
	frame = 0;
	this->clipTexture(85, 61, 12, 28, false);
}