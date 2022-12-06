#include "player.h"

// ==================================
//  		Public Functions
// ==================================

// Constructer
Player::Player() {
	this->team = "Player";
	this->x = 5;
	this->y = 8;
}

// ===================================
//  		Private Functions
// ===================================

// General Input and Deciding which Move to do
void Player::movement(float _dt) {

	if (this->kMouseLP) {
		if (this->piece == "King") {
			if (this->kMouse.x / this->mousePosMod == this->moveset[0][0] and this->kMouse.y / this->mousePosMod == this->moveset[1][0]) {
				this->kUpP = true;
			}
			else if (this->kMouse.x / this->mousePosMod == this->moveset[0][1] and this->kMouse.y / this->mousePosMod == this->moveset[1][1]) {
				this->kDownP = true;
			}
			else if (this->kMouse.x / this->mousePosMod == this->moveset[0][2] and this->kMouse.y / this->mousePosMod == this->moveset[1][2]) {
				this->kLeftP = true;
			}
			else if (this->kMouse.x / this->mousePosMod == this->moveset[0][3] and this->kMouse.y / this->mousePosMod == this->moveset[1][3]) {
				this->kRightP = true;
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
			if (this->kMouse.x > Global::WIN_WIDTH / 2 + 4 and this->kMouse.x < Global::WIN_WIDTH / 2 + 30 and this->kMouse.y < Global::WIN_HEIGHT / 2 + 4) { // Up
					this->kUpP = true;
			}
		}
	}
	else {
		this->kUpP = false;
		this->kDownP = false;
		this->kLeftP = false;
		this->kRightP = false;
	}
	this->isSelected = false;

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
	else if (this->piece == "Queen") {

	}
	else if (this->piece == "Bishop") {
		
	}
	else if (this->piece == "Knight") {
		
	}
	else if (this->piece == "Rook") {
		
	}
	else if (this->piece == "Pawn") {
		if (this->kUpP) {
			this->dir = 0;
			this->doStep = true;
		}
	}
}

// For Changing the Player's Sprite
void Player::setAnimation(std::string _animation) {
	if (_animation == "King")
		this->clipTexture(85, 61, 12, 28, false);
	else if (_animation == "Queen")
		this->clipTexture(71, 61, 12, 28, false);
	else if (_animation == "Bishop")
		this->clipTexture(43, 61, 12, 28, false);
	else if (_animation == "Knight")
		this->clipTexture(29, 61, 12, 28, false);
	else if (_animation == "Rook")
		this->clipTexture(57, 61, 12, 28, false);
	else if (_animation == "Pawn")
		this->clipTexture(15, 61, 12, 28, false);
}
