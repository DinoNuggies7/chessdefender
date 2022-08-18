#include "player.h"

// ==================================
//  		Public Functions
// ==================================

Player::Player() {
	this->team = "Player";
	this->x = 5;
	this->y = 8;
}

// ===================================
//  		Private Functions
// ===================================

void Player::movement(float _dt) {
	if (this->piece == "King") {
		if (this->kUpP) {
			this->dir = 0;
			this->doStep = true;
		}
		else if (this->kDownP) {
			this->dir = 1;
			this->doStep = true;
		}
		else if (this->kLeftP) {
			this->dir = 2;
			this->doStep = true;
		}
		else if (this->kRightP) {
			this->dir = 3;
			this->doStep = true;
		}
	}
}

void Player::setAnimation(std::string _animation) {
	if (_animation == "King")
		this->clipTexture(85, 61, 12, 28, false);
	else if (_animation == "Queen")
		this->clipTexture(71, 61, 12, 28, false);
	else if (_animation == "Bishop")
		this->clipTexture(53, 61, 12, 28, false);
	else if (_animation == "Knight")
		this->clipTexture(29, 61, 12, 28, false);
	else if (_animation == "Rook")
		this->clipTexture(57, 61, 12, 28, false);
	else if (_animation == "Pawn")
		this->clipTexture(15, 61, 12, 28, false);
}