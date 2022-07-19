#include "entity.h"

// Public Functions
void Entity::init(const char* _spritePath) {
	this->x = 0;
	this->y = 0;
	this->vx = 0;
	this->vy = 0;
	this->frame = 0;

	this->initSprite(_spritePath);
	this->setAnimation("stand");
}

void Entity::update(float _dt) {
	this->input();
	this->physics(_dt);
}

void Entity::render(sf::RenderWindow* _window) {
	_window->draw(this->sprite);
}

// Protected Functions
void Entity::physics(float _dt) {
	this->x += this->vx;
	this->y += this->vy;
	this->sprite.setPosition(this->x, this->y);

	this->collision(_dt);
}

void Entity::collision(float _dt) {

}

void Entity::initSprite(const char* _spritePath) {
	this->image.loadFromFile(_spritePath);
	this->image.createMaskFromColor(sf::Color(0x6f6d51ff));
	this->image.createMaskFromColor(sf::Color(0xf9303dff));
	this->texture.loadFromImage(this->image);
	this->sprite.setTexture(this->texture);
}

void Entity::setAnimation(const char* _animation) {
	frame = 0;
	if (_animation == "stand") {
		this->clipTexture(85, 61, 12, 28, false);
	}
}

void Entity::clipTexture(int _x, int _y, int _w, int _h, bool _flip = false) {
	if (_flip)
		this->sprite.setTextureRect(sf::IntRect(_x+_w, _y, -_w, _h));
	else
		this->sprite.setTextureRect(sf::IntRect(_x, _y, _w, _h));
}