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
		this->setMoveset("King");
		this->piece = "King";
	}
	else if (_piece == "Queen") {
		this->setAnimation("Queen");
		this->setMoveset("Queen");
		this->piece = "Queen";
	}
	else if (_piece == "Bishop") {
		this->setAnimation("Bishop");
		this->setMoveset("Bishop");
		this->piece = "Bishop";
	}
	else if (_piece == "Knight") {
		this->setAnimation("Knight");
		this->setMoveset("Knight");
		this->piece = "Knight";
	}
	else if (_piece == "Rook") {
		this->setAnimation("Rook");
		this->setMoveset("Rook");
		this->piece = "Rook";
	}
	else if (_piece == "Pawn") {
		this->setAnimation("Pawn");
		this->setMoveset("Pawn");
		this->piece = "Pawn";
	}
}

void Entity::update(sf::RenderWindow& _window, float _dt) {
	this->input(_window);
	this->physics(_dt);
}

void Entity::render(sf::RenderWindow& _window) {
	_window.draw(this->sprite);
	if (this->isSelected) {
		if (this->piece == "King" or this->piece == "Rook" or this->piece == "Bishop") {
			for (int i = 0; i < 4; i++) {
				this->movesetHighlight[i].setPosition(sf::Vector2f((this->x + this->moveset[i][0]) * 16, (this->y + this->moveset[i][1]) * 16));
				_window.draw(this->movesetHighlight[i]);
			}
		}
		else if (this->piece == "Queen") {
			for (int i = 0; i < 8; i++) {
				this->movesetHighlight[i].setPosition(sf::Vector2f((this->x + this->moveset[i][0]) * 16, (this->y + this->moveset[i][1]) * 16));
				_window.draw(this->movesetHighlight[i]);
			}
		}
		else if (this->piece == "Pawn") {
			this->movesetHighlight[0].setPosition(sf::Vector2f((this->x + this->moveset[0][0]) * 16, (this->y + this->moveset[0][1]) * 16));
			_window.draw(this->movesetHighlight[0]);
		}
	}
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
	for (int i = 0; i < 8; i++) {
		this->movesetHighlight[i].setSize(sf::Vector2f(16, 16));
		this->movesetHighlight[i].setFillColor(sf::Color(255, 255, 0, 50));
	}
}

void Entity::setAnimation(std::string _animation) {

}

void Entity::setMoveset(std::string _piece) {
	if (_piece == "King") {
		this->moveset[0][0] = 0;
		this->moveset[0][1] = -1;

		this->moveset[1][0] = 0;
		this->moveset[1][1] = 1;

		this->moveset[2][0] = -1;
		this->moveset[2][1] = 0;

		this->moveset[3][0] = 1;
		this->moveset[3][1] = 0;
	}
	else if (_piece == "Queen") {
		this->moveset[0][0] = 0;
		this->moveset[0][1] = -3;

		this->moveset[1][0] = 0;
		this->moveset[1][1] = 3;

		this->moveset[2][0] = -3;
		this->moveset[2][1] = 0;

		this->moveset[3][0] = 3;
		this->moveset[3][1] = 0;

		this->moveset[4][0] = -3;
		this->moveset[4][1] = -3;

		this->moveset[5][0] = -3;
		this->moveset[5][1] = 3;

		this->moveset[6][0] = 3;
		this->moveset[6][1] = -3;

		this->moveset[7][0] = 3;
		this->moveset[7][1] = 3;
	}
	else if (_piece == "Bishop") {
		this->moveset[0][0] = -3;
		this->moveset[0][1] = -3;

		this->moveset[1][0] = -3;
		this->moveset[1][1] = 3;

		this->moveset[2][0] = 3;
		this->moveset[2][1] = -3;

		this->moveset[3][0] = 3;
		this->moveset[3][1] = 3;
	}
	else if (_piece == "Knight") {
		this->moveset[0][0] = -1;
		this->moveset[0][1] = -2;

		this->moveset[1][0] = 1;
		this->moveset[1][1] = -2;

		this->moveset[2][0] = 2;
		this->moveset[2][1] = -1;

		this->moveset[3][0] = 2;
		this->moveset[3][1] = 1;

		this->moveset[4][0] = 1;
		this->moveset[4][1] = 2;

		this->moveset[5][0] = -1;
		this->moveset[5][1] = 2;

		this->moveset[6][0] = -2;
		this->moveset[6][1] = 1;

		this->moveset[7][0] = -2;
		this->moveset[7][1] = -1;
	}
	else if (_piece == "Rook") {
		this->moveset[0][0] = 0;
		this->moveset[0][1] = -2;

		this->moveset[1][0] = 0;
		this->moveset[1][1] = 2;

		this->moveset[2][0] = -2;
		this->moveset[2][1] = 0;

		this->moveset[3][0] = 2;
		this->moveset[3][1] = 0;
	}
	else if (_piece == "Pawn") {
		this->moveset[0][0] = 0;
		this->moveset[0][1] = -1;
	}
}

void Entity::clipTexture(int _x, int _y, int _w, int _h, bool _flip = false) {
	if (_flip)
		this->sprite.setTextureRect(sf::IntRect(_x+_w, _y, -_w, _h));
	else
		this->sprite.setTextureRect(sf::IntRect(_x, _y, _w, _h));
}