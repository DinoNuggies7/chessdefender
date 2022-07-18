#include "entity.h"

// Public Functions
void Entity::init(const char* _spritePath) {
	this->initSprite(_spritePath);
}

void Entity::update(float _dt) {

}

void Entity::render(sf::RenderWindow* _window) {
	_window->draw(this->sprite);
}

// Protected Functions
void Entity::initSprite(const char* _spritePath) {
	this->image.loadFromFile(_spritePath);
	this->image.createMaskFromColor(sf::Color(0x6f6d51ff));
	this->image.createMaskFromColor(sf::Color(0xf9303dff));
	this->texture.loadFromImage(this->image);
	this->sprite.setTexture(this->texture);
}

void Entity::setAnimation() {
	
}