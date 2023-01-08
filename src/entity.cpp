#include "entity.h"

// ==================================
//  		Public Functions
// ==================================

// Deconstructor
Entity::~Entity() {
	
}

// Standard Initializations
void Entity::init(std::string _entity) {
	// Entity Variables
	this->dir = -1;
	this->doStep = false;
	this->turn = false;
	this->inCheck = false;
	this->offsetX = 0;
	this->offsetY = 0;

	// Set the Enitity's Sprite, Moveset, Spritesheet, and `piece` Variable based on the Initialized Argument
	if (_entity == "King") {
		this->initSprite("assets/pieces.png");
		this->setAnimation("King");
		this->piece = "King";
	}
	else if (_entity == "Queen") {
		this->initSprite("assets/pieces.png");
		this->setAnimation("Queen");
		this->piece = "Queen";
	}
	else if (_entity == "Bishop") {
		this->initSprite("assets/pieces.png");
		this->setAnimation("Bishop");
		this->piece = "Bishop";
	}
	else if (_entity == "Knight") {
		this->initSprite("assets/pieces.png");
		this->setAnimation("Knight");
		this->piece = "Knight";
	}
	else if (_entity == "Rook") {
		this->initSprite("assets/pieces.png");
		this->setAnimation("Rook");
		this->piece = "Rook";
	}
	else if (_entity == "Pawn") {
		this->initSprite("assets/pieces.png");
		this->setAnimation("Pawn");
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
	if (this->team != "Object")
		this->sprite.setPosition(this->x * 16 + 2 + this->offsetX, this->y * 16 - 13 + this->offsetY);
	else
		this->sprite.setPosition(this->x * 16, this->y * 16);

	if (this->turn)
		this->movement(_dt);
}

// General Chess Movement Logic
void Entity::collision(std::vector<std::vector<int>>& _mapLayerCollision) {
	this->moveset[0].clear();
	this->moveset[1].clear();
	this->moves = 0;
	if (this->piece == "King") {
		if (this->y - 1 > -1 and this->x - 1 > -1 and _mapLayerCollision[int(this->y)-1+1][int(this->x)-1] == 0) { // Up Left
			this->moveset[0].push_back(int(this->x)-1);
			this->moveset[1].push_back(int(this->y)-1);
			this->moves++;
		}
		else {
			this->moveset[0].push_back(-1);
			this->moveset[1].push_back(-1);
			this->moves++;
		}
		if (this->y - 1 > -1 and this->x > -1 and _mapLayerCollision[int(this->y)-1+1][int(this->x)] == 0) { // Up
			this->moveset[0].push_back(int(this->x));
			this->moveset[1].push_back(int(this->y)-1);
			this->moves++;
		}
		else {
			this->moveset[0].push_back(-1);
			this->moveset[1].push_back(-1);
			this->moves++;
		}
		if (this->y - 1 > -1 and this->x + 1 > -1 and _mapLayerCollision[int(this->y)-1+1][int(this->x)+1] == 0) { // Up Right
			this->moveset[0].push_back(int(this->x)+1);
			this->moveset[1].push_back(int(this->y)-1);
			this->moves++;
		}
		else {
			this->moveset[0].push_back(-1);
			this->moveset[1].push_back(-1);
			this->moves++;
		}
		if (this->y > -1 and this->x + 1 > -1 and _mapLayerCollision[int(this->y)+1][int(this->x)+1] == 0) { // Right
			this->moveset[0].push_back(int(this->x)+1);
			this->moveset[1].push_back(int(this->y));
			this->moves++;
		}
		else {
			this->moveset[0].push_back(-1);
			this->moveset[1].push_back(-1);
			this->moves++;
		}
		if (this->y + 1 > -1 and this->x + 1 > -1 and this->y + 1 < 16 and _mapLayerCollision[int(this->y)+1+1][int(this->x)+1] == 0) { // Down Right
			this->moveset[0].push_back(int(this->x)+1);
			this->moveset[1].push_back(int(this->y)+1);
			this->moves++;
		}
		else {
			this->moveset[0].push_back(-1);
			this->moveset[1].push_back(-1);
			this->moves++;
		}
		if (this->y + 1 > -1 and this->x > -1 and this->y + 1 < 16 and  _mapLayerCollision[int(this->y)+1+1][int(this->x)] == 0) { // Down
			this->moveset[0].push_back(int(this->x));
			this->moveset[1].push_back(int(this->y)+1);
			this->moves++;
		}
		else {
			this->moveset[0].push_back(-1);
			this->moveset[1].push_back(-1);
			this->moves++;
		}
		if (this->y + 1 > -1 and this->x - 1 > -1 and this->y + 1 < 16 and  _mapLayerCollision[int(this->y)+1+1][int(this->x)-1] == 0) { // Down Left
			this->moveset[0].push_back(int(this->x)-1);
			this->moveset[1].push_back(int(this->y)+1);
			this->moves++;
		}
		else {
			this->moveset[0].push_back(-1);
			this->moveset[1].push_back(-1);
			this->moves++;
		}
		if (this->y > -1 and this->x - 1 > -1 and _mapLayerCollision[int(this->y)+1][int(this->x)-1] == 0) { // Left
			this->moveset[0].push_back(int(this->x)-1);
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
		if (_mapLayerCollision[int(this->y)-2+1][int(this->x)-1] == 0 and this->y - 2 > -1 and this->x - 1 > -1) { // Up Left
			this->moveset[0].push_back(int(this->x)-1);
			this->moveset[1].push_back(int(this->y)-2);
			this->moves++;
		}
		else {
			this->moveset[0].push_back(-1);
			this->moveset[1].push_back(-1);
			this->moves++;
		}
		if (_mapLayerCollision[int(this->y)-2+1][int(this->x)+1] == 0 and this->y - 2 > -1 and this->x + 1 > -1) { // Up Right
			this->moveset[0].push_back(int(this->x)+1);
			this->moveset[1].push_back(int(this->y)-2);
			this->moves++;
		}
		else {
			this->moveset[0].push_back(-1);
			this->moveset[1].push_back(-1);
			this->moves++;
		}
		if (_mapLayerCollision[int(this->y)-1+1][int(this->x)+2] == 0 and this->y - 1 > -1 and this->x + 2 > -1) { // Right Up
			this->moveset[0].push_back(int(this->x)+2);
			this->moveset[1].push_back(int(this->y)-1);
			this->moves++;
		}
		else {
			this->moveset[0].push_back(-1);
			this->moveset[1].push_back(-1);
			this->moves++;
		}
		if (_mapLayerCollision[int(this->y)+1+1][int(this->x)+2] == 0 and this->y + 1 > -1 and this->x + 2 > -1) { // Right Down
			this->moveset[0].push_back(int(this->x)+2);
			this->moveset[1].push_back(int(this->y)+1);
			this->moves++;
		}
		else {
			this->moveset[0].push_back(-1);
			this->moveset[1].push_back(-1);
			this->moves++;
		}
		if (_mapLayerCollision[int(this->y)+2+1][int(this->x)+1] == 0 and this->y + 2 > -1 and this->x + 1 > -1) { // Down Right
			this->moveset[0].push_back(int(this->x)+1);
			this->moveset[1].push_back(int(this->y)+2);
			this->moves++;
		}
		else {
			this->moveset[0].push_back(-1);
			this->moveset[1].push_back(-1);
			this->moves++;
		}
		if (_mapLayerCollision[int(this->y)+2+1][int(this->x)-1] == 0 and this->y + 2 > -1 and this->x - 1 > -1) { // Down Left
			this->moveset[0].push_back(int(this->x)-1);
			this->moveset[1].push_back(int(this->y)+2);
			this->moves++;
		}
		else {
			this->moveset[0].push_back(-1);
			this->moveset[1].push_back(-1);
			this->moves++;
		}
		if (_mapLayerCollision[int(this->y)+1+1][int(this->x)-2] == 0 and this->y + 1 > -1 and this->x - 2 > -1) { // Left Down
			this->moveset[0].push_back(int(this->x)-2);
			this->moveset[1].push_back(int(this->y)+1);
			this->moves++;
		}
		else {
			this->moveset[0].push_back(-1);
			this->moveset[1].push_back(-1);
			this->moves++;
		}
		if (_mapLayerCollision[int(this->y)-1+1][int(this->x)-2] == 0 and this->y - 1 > -1 and this->x - 2 > -1) { // Left Up
			this->moveset[0].push_back(int(this->x)-2);
			this->moveset[1].push_back(int(this->y)-1);
			this->moves++;
		}
		else {
			this->moveset[0].push_back(-1);
			this->moveset[1].push_back(-1);
			this->moves++;
		}
	}
	else if (this->piece == "Rook") {
		
	}
	else if (this->piece == "Pawn") {
		if (_mapLayerCollision[int(this->y)-1+1][int(this->x)-1] == 0 and this->y - 1 > -1 and this->x - 1 > -1) { // Up Left
			this->moveset[0].push_back(int(this->x)-1);
			this->moveset[1].push_back(int(this->y)-1);
			this->moves++;
		}
		else {
			this->moveset[0].push_back(-1);
			this->moveset[1].push_back(-1);
			this->moves++;
		}
		if (_mapLayerCollision[int(this->y)-1+1][int(this->x)] == 0 and this->y - 1 > -1 and this->x > -1) { // Up
			this->moveset[0].push_back(int(this->x));
			this->moveset[1].push_back(int(this->y)-1);
			this->moves++;
		}
		else {
			this->moveset[0].push_back(-1);
			this->moveset[1].push_back(-1);
			this->moves++;
		}
		if (_mapLayerCollision[int(this->y)-1+1][int(this->x)+1] == 0 and this->y - 1 > -1 and this->x + 1 > -1) { // Up Right
			this->moveset[0].push_back(int(this->x)+1);
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
void Entity::initSprite(std::string _file) {
	this->image.loadFromFile(_file);
	this->image.createMaskFromColor(sf::Color(0x6f6d51ff));
	this->image.createMaskFromColor(sf::Color(0xf9303dff));
	this->texture.loadFromImage(this->image);
	this->sprite.setTexture(this->texture);
}

// For Changing the Entity's Sprite
void Entity::setAnimation(std::string _animation) {

}

// So I don't have to type as much
void Entity::clipTexture(int _x, int _y, int _w, int _h, bool _flip = false) {
	if (_flip)
		this->sprite.setTextureRect(sf::IntRect(_x+_w, _y, -_w, _h));
	else
		this->sprite.setTextureRect(sf::IntRect(_x, _y, _w, _h));
}