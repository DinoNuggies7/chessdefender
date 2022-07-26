#include "entity.h"

// ==================
//  Public Functions
// ==================

void Entity::init() {
	this->vx = 0;
	this->vy = 0;
	this->dir = -1;
	this->frame = 0;
	this->doStep = 0;
	this->turn = false;

	this->initSprite();
	this->setAnimation("stand");
}

void Entity::update(float _dt) {
	this->input();
	this->physics(_dt);
}

void Entity::render(sf::RenderWindow* _window) {
	_window->draw(this->sprite);
}

// =====================
//  Protected Functions
// =====================

void Entity::physics(float _dt) {
	this->x += this->vx;
	this->y += this->vy;
	this->sprite.setPosition((this->x * 16) + 2, (this->y * 16) - 13);
	if (this->turn)
		this->movement(_dt);
	this->collision(_dt);
}

void Entity::movement(float _dt) {

}

void Entity::collision(float _dt) {

}

void Entity::initSprite() {

}

void Entity::setAnimation(const char* _animation) {

}

void Entity::clipTexture(int _x, int _y, int _w, int _h, bool _flip = false) {
	if (_flip)
		this->sprite.setTextureRect(sf::IntRect(_x+_w, _y, -_w, _h));
	else
		this->sprite.setTextureRect(sf::IntRect(_x, _y, _w, _h));
}