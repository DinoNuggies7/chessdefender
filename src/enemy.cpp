#include "enemy.h"

// ==================================
//  		Public Functions
// ==================================

Enemy::Enemy() {
	this->team = "Enemy";
	this->x = 0;
	this->y = 0;
}


// ===================================
//  		Private Functions
// ===================================

void Enemy::movement(float _dt) {
	int _rand;
	if (this->piece == "King")
		_rand = std::rand() % 4;
	else if (this->piece == "Queen")
		_rand = std::rand() % 8;
	else if (this->piece == "Bishop")
		_rand = std::rand() % 4;
	else if (this->piece == "Knight")
		_rand = std::rand() % 8;
	else if (this->piece == "Rook")
		_rand = std::rand() % 4;
	else if (this->piece == "Pawn")
		_rand = 0;
	this->dir = _rand;
	this->doStep = true;
}

void Enemy::setAnimation(std::string _animation) {
	if (_animation == "King")
		this->clipTexture(85, 31, 12, 28, false);
	else if (_animation == "Queen")
		this->clipTexture(71, 31, 12, 28, false);
	else if (_animation == "Bishop")
		this->clipTexture(43, 31, 12, 28, false);
	else if (_animation == "Knight")
		this->clipTexture(29, 31, 12, 28, false);
	else if (_animation == "Rook")
		this->clipTexture(57, 31, 12, 28, false);
	else if (_animation == "Pawn")
		this->clipTexture(15, 31, 12, 28, false);
}