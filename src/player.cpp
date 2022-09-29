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

	if (this->kMouseL) {
		if (this->kMouse.x > Global::WIN_WIDTH / 2 + 4 and this->kMouse.x < Global::WIN_WIDTH / 2 + 30 and this->kMouse.y > Global::WIN_HEIGHT / 2 + 4 and this->kMouse.y < Global::WIN_HEIGHT / 2 + 32) {
			this->isSelected = true;
		}
	}
	else {
		if (this->isSelected) {
			if (this->kMouse.x > Global::WIN_WIDTH / 2 + 4 and this->kMouse.x < Global::WIN_WIDTH / 2 + 30 and this->kMouse.y < Global::WIN_HEIGHT / 2 + 4) { // Up
				this->kUpP = true;
			}
			else if (this->kMouse.x > Global::WIN_WIDTH / 2 + 4 and this->kMouse.x < Global::WIN_WIDTH / 2 + 30 and this->kMouse.y > Global::WIN_HEIGHT / 2 + 32) { // Down
				this->kDownP = true;
			}
			else if (this->kMouse.x < Global::WIN_WIDTH / 2 + 4 and this->kMouse.y > Global::WIN_HEIGHT / 2 + 4 and this->kMouse.y < Global::WIN_HEIGHT / 2 + 32) { // Left
				this->kLeftP = true;
			}
			else if (this->kMouse.x > Global::WIN_WIDTH / 2 + 30 and this->kMouse.y > Global::WIN_HEIGHT / 2 + 4 and this->kMouse.y < Global::WIN_HEIGHT / 2 + 32) { // Right
				this->kRightP = true;
			}
		}
		else {
			this->kUpP = false;
			this->kDownP = false;
			this->kLeftP = false;
			this->kRightP = false;
		}
		this->isSelected = false;
	}

	if (this->isSelected) {
		this->offsetX = this->kMouse.x / 2 - 248;
		this->offsetY = this->kMouse.y / 2 - 143;
	}
	else {
		this->offsetX = 0;
		this->offsetY = 0;
	}
	
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