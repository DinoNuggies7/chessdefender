#include "enemy.h"

// ==================================
//  		Public Functions
// ==================================

// Constructor
Enemy::Enemy() {
	this->team = "Enemy";
	this->x = 0;
	this->y = 0;
}


// ===================================
//  		Private Functions
// ===================================

// For Deciding a Move to make
void Enemy::movement(float _dt) {
	int _rand = -1;

	for (int i = 0; i < this->moves; i++) {
		if (this->canTake[0][i] != 0 and this->canTake[1][i] != 0)
			_rand = i;
	}
	if (_rand == -1)
		_rand = std::rand() % this->moves;

	this->dir = _rand;
	this->doStep = true;
}

// For changing the Enemy's Sprite
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