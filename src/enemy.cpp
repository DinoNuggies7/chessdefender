#include "enemy.h"

// ==================
//  Public Functions
// ==================

Enemy::Enemy() {
	this->team = "Enemy";
	this->x = 13;
	this->y = 5;
}

// =====================
//  Protected Functions
// =====================

void Enemy::movement(float _dt) {
	int _rand = std::rand() % 4;
	switch (_rand) {
		case 0:
			this->dir = 0;
			this->doStep = true;
			break;
		case 1:
			this->dir = 1;
			this->doStep = true;
			break;
		case 2:
			this->dir = 2;
			this->doStep = true;
			break;
		case 3:
			this->dir = 3;
			this->doStep = true;
			break;
	}
}

void Enemy::initSprite() {
	this->image.loadFromFile("assets/pieces.png");
	this->image.createMaskFromColor(sf::Color(0x6f6d51ff));
	this->image.createMaskFromColor(sf::Color(0xf9303dff));
	this->texture.loadFromImage(this->image);
	this->sprite.setTexture(this->texture);
}

void Enemy::setAnimation(const char* _animation) {
	frame = 0;
	this->clipTexture(85, 31, 12, 28, false);
}