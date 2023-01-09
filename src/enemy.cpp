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
	int _dir = -1;
	this->dirLengthX = 1;
	this->dirLengthY = 1;

	for (int i = 0; i < this->moves; i++) {
		if (this->canTake[0][i] != 0 and this->canTake[1][i] != 0) {
			_dir = i;
			this->dirLengthX = this->x - this->canTake[0][i];
			this->dirLengthY = this->y - this->canTake[1][i];
		}
	}

	if (_dir == -1)
		_dir = std::rand() % this->moves;

	this->dir = _dir;
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