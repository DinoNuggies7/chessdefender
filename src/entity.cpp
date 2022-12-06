#include "entity.h"

// ==================================
//  		Public Functions
// ==================================

// Standard Initializations
void Entity::init(std::string _piece) {
	// Entity Variables
	this->vx = 0;
	this->vy = 0;
	this->dir = -1;
	this->doStep = 0;
	this->turn = false;

	// Initialize the Entity's Sprite
	this->initSprite();

	// Set the Enitity's Sprite, Moveset, and `piece` Variable based on the Initialized Argument
	if (_piece == "King") {
		this->setAnimation("King");
		//this->setMoveset("King");
		this->piece = "King";
	}
	else if (_piece == "Queen") {
		this->setAnimation("Queen");
		//this->setMoveset("Queen");
		this->piece = "Queen";
	}
	else if (_piece == "Bishop") {
		this->setAnimation("Bishop");
		//this->setMoveset("Bishop");
		this->piece = "Bishop";
	}
	else if (_piece == "Knight") {
		this->setAnimation("Knight");
		//this->setMoveset("Knight");
		this->piece = "Knight";
	}
	else if (_piece == "Rook") {
		this->setAnimation("Rook");
		//this->setMoveset("Rook");
		this->piece = "Rook";
	}
	else if (_piece == "Pawn") {
		this->setAnimation("Pawn");
		//this->setMoveset("Pawn");
		this->piece = "Pawn";
	}
}

// This Runs Every Frame
void Entity::update(sf::RenderWindow& _window, float& _dt, std::vector<std::vector<int>>& _mapLayerCollision) {
	this->input(_window);
	this->physics(_dt);
	this->collision(_mapLayerCollision);
}

// Drawing the Entity's Sprite to the Window
void Entity::render(sf::RenderWindow& _window) {
	// Rendering Highlights for Possible Moves
	if (this->team == "Player") {
		this->movesetHighlight.clear();
		for (int i = 0; i < this->moves; i++) {
			this->movesetHighlight.push_back(sf::RectangleShape(sf::Vector2f(16, 16)));
			this->movesetHighlight[i].setFillColor(sf::Color(25, 255, 25, 25));
			this->movesetHighlight[i].setPosition(this->moveset[0][i] * 16, this->moveset[1][i] * 16);
			_window.draw(this->movesetHighlight[i]);
		}
	}

	// Drawing the Entity's Sprite
	_window.draw(this->sprite);
}

// =====================================
//  		Protected Functions
// =====================================

// General Physics
void Entity::physics(float _dt) {
	this->x += this->vx;
	this->y += this->vy;
	this->sprite.setPosition(this->x * 16 + 2 + this->offsetX, this->y * 16 - 13 + this->offsetY);
	if (this->turn)
		this->movement(_dt);
}

// General Chess Movement Logic
void Entity::collision(std::vector<std::vector<int>>& _mapLayerCollision) {
	this->moveset[0].clear();
	this->moveset[1].clear();
	this->moves = 0;
	if (this->piece == "King") {
		if (_mapLayerCollision[int(this->y)-1+1][int(this->x)] == 0) {
			this->moveset[0].push_back(int(this->x));
			this->moveset[1].push_back(int(this->y)-1);
			this->moves++;
		}
		else {
			this->moveset[0].push_back(-1);
			this->moveset[1].push_back(-1);
			this->moves++;
		}
		if (_mapLayerCollision[int(this->y)+1+1][int(this->x)] == 0) {
			this->moveset[0].push_back(int(this->x));
			this->moveset[1].push_back(int(this->y)+1);
			this->moves++;
		}
		else {
			this->moveset[0].push_back(-1);
			this->moveset[1].push_back(-1);
			this->moves++;
		}
		if (_mapLayerCollision[int(this->y)+1][int(this->x)-1] == 0) {
			this->moveset[0].push_back(int(this->x)-1);
			this->moveset[1].push_back(int(this->y));
			this->moves++;
		}
		else {
			this->moveset[0].push_back(-1);
			this->moveset[1].push_back(-1);
			this->moves++;
		}
		if (_mapLayerCollision[int(this->y)+1][int(this->x)+1] == 0) {
			this->moveset[0].push_back(int(this->x)+1);
			this->moveset[1].push_back(int(this->y));
			this->moves++;
		}
		else {
			this->moveset[0].push_back(-1);
			this->moveset[1].push_back(-1);
			this->moves++;
		}
	}
	else if (this->piece == "Queen") {

	}
	else if (this->piece == "Bishop") {
		
	}
	else if (this->piece == "Knight") {
		
	}
	else if (this->piece == "Rook") {
		
	}
	else if (this->piece == "Pawn") {
		if (_mapLayerCollision[int(this->y)-1+1][int(this->x)] == 0) {
			this->moveset[0].push_back(int(this->x));
			this->moveset[1].push_back(int(this->y)-1);
			this->moves++;
		}
		else {
			this->moveset[0].push_back(-1);
			this->moveset[1].push_back(-1);
			this->moves++;
		}
	}
}

// General Input Logic
void Entity::movement(float _dt) {

}

// Function for Initializing the Entity's Sprite
void Entity::initSprite() {
	this->image.loadFromFile("assets/pieces.png");
	this->image.createMaskFromColor(sf::Color(0x6f6d51ff));
	this->image.createMaskFromColor(sf::Color(0xf9303dff));
	this->texture.loadFromImage(this->image);
	this->sprite.setTexture(this->texture);
}

// For Changing the Entity's Sprite
void Entity::setAnimation(std::string _animation) {

}

// Just for Cleanlyness, the Movesets are Defined Here instead of `Entity::init()`
void Entity::setMoveset(std::string _piece) {
	if (_piece == "King") {
		this->moveset[0][0] = 0;
		this->moveset[0][0] = -1;

		this->moveset[1][1] = 0;
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
		this->moveset[0][1] = -3;

		this->moveset[1][0] = 0;
		this->moveset[1][1] = 3;

		this->moveset[2][0] = -3;
		this->moveset[2][1] = 0;

		this->moveset[3][0] = 3;
		this->moveset[3][1] = 0;
	}
	else if (_piece == "Pawn") {
		this->moveset[0][0] = 0;
		this->moveset[0][1] = -1;
	}
}

// So I don't have to type as much
void Entity::clipTexture(int _x, int _y, int _w, int _h, bool _flip = false) {
	if (_flip)
		this->sprite.setTextureRect(sf::IntRect(_x+_w, _y, -_w, _h));
	else
		this->sprite.setTextureRect(sf::IntRect(_x, _y, _w, _h));
}