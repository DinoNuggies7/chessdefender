#include "entity.h"

// Public Functions
void Entity::init(const char* _spritePath) {
	this->initSprite(_spritePath);
	this->setAnimation("stand.east");
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

	switch (this->dir) {
		case 0:
			if (this->kInput)
				setAnimation("walk.north");
			else
				setAnimation("stand.north");
			break;
		case 1:
			if (this->kInput)
				setAnimation("walk.northeast");
			else
				setAnimation("stand.northeast");
			break;
		case 2:
			//if (this->kInput)
			//	setAnimation("walk.east");
			//else
				setAnimation("stand.east");
			break;
		case 3:
			if (this->kInput)
				setAnimation("walk.southeast");
			else
				setAnimation("stand.southeast");
			break;
		case 4:
			if (this->kInput)
				setAnimation("walk.south");
			else
				setAnimation("stand.south");
			break;
		case 5:
			if (this->kInput)
				setAnimation("walk.southwest");
			else
				setAnimation("stand.southwest");
			break;
		case 6:
			//if (this->kInput)
			//	setAnimation("walk.west");
			//else
				setAnimation("stand.west");
			break;
		case 7:
			if (this->kInput)
				setAnimation("walk.northwest");
			else
				setAnimation("stand.northwest");
			break;
	}

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

void Entity::clipTexture(int _x, int _y, int _w, int _h, bool _flip = false) {
	if (_flip)
		this->sprite.setTextureRect(sf::IntRect(_x+_w, _y, -_w, _h));
	else
		this->sprite.setTextureRect(sf::IntRect(_x, _y, _w, _h));
}

void Entity::setAnimation(const char* _animation) {
	frame = 0;
	if (_animation == "stand.north") {
		this->clipTexture(2, 2, 16, 16);
	}
	else if (_animation == "stand.east") {
		this->clipTexture(21, 2, 16, 16);
	}
	else if (_animation == "stand.south") {
		this->clipTexture(40, 2, 16, 16);
	}
	else if (_animation == "stand.west") {
		this->clipTexture(21, 2, 16, 16, true);
	}
}