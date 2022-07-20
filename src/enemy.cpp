#include "enemy.h"

// Protected Functions
void Enemy::movement(float _dt) {
	int _rand = std::rand() % 4;
	switch (_rand) {
		case 0:
			this->y -= 16;
			break;
		case 1:
			this->y += 16;
			break;
		case 2:
			this->x -= 16;
			break;
		case 3:
			this->x += 16;
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