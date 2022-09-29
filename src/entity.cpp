#include "entity.h"

// ==================================
//  		Public Functions
// ==================================

void Entity::init(std::string _piece) {
	this->vx = 0;
	this->vy = 0;
	this->dir = -1;
	this->doStep = 0;
	this->turn = false;

	this->initSprite();

	if (_piece == "King") {
		this->setAnimation("King");
		this->piece = "King";
	}
	else if (_piece == "Queen") {
		this->setAnimation("Queen");
		this->piece = "Queen";
	}
	else if (_piece == "Bishop") {
		this->setAnimation("Bishop");
		this->piece = "Bishop";
	}
	else if (_piece == "Knight") {
		this->setAnimation("Knight");
		this->piece = "Knight";
	}
	else if (_piece == "Rook") {
		this->setAnimation("Rook");
		this->piece = "Rook";
	}
	else if (_piece == "Pawn") {
		this->setAnimation("Pawn");
		this->piece = "Pawn";
	}
}

void Entity::update(sf::RenderWindow& _window, float _dt) {
	this->input(_window);
	this->physics(_dt);
}

void Entity::render(sf::RenderWindow& _window) {
	_window.draw(this->sprite);
}

// =====================================
//  		Protected Functions
// =====================================

void Entity::physics(float _dt) {
	this->x += this->vx;
	this->y += this->vy;
	this->sprite.setPosition(this->x * 16 + 2 + this->offsetX, this->y * 16 - 13 + this->offsetY);
	if (this->turn)
		this->movement(_dt);
	this->collision(_dt);
}

void Entity::movement(float _dt) {

}

void Entity::collision(float _dt) {

}

void Entity::initSprite() {
	this->image.loadFromFile("assets/pieces.png");
	this->image.createMaskFromColor(sf::Color(0x6f6d51ff));
	this->image.createMaskFromColor(sf::Color(0xf9303dff));
	this->texture.loadFromImage(this->image);
	this->sprite.setTexture(this->texture);
}

void Entity::setAnimation(std::string _animation) {

}

void Entity::clipTexture(int _x, int _y, int _w, int _h, bool _flip = false) {
	if (_flip)
		this->sprite.setTextureRect(sf::IntRect(_x+_w, _y, -_w, _h));
	else
		this->sprite.setTextureRect(sf::IntRect(_x, _y, _w, _h));
}