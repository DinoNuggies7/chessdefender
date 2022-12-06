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
			for (int i = 0; i < this->moves; i++) {
				if (int(this->kMouse.x / this->mousePosMod) == this->moveset[0][i] and int(this->kMouse.y / this->mousePosMod) == this->moveset[1][i]) {
					this->doStep = true;
					this->dir = i;
				}
			}
		}
		else if (this->piece == "Queen") {

			}
		else if (this->piece == "Bishop") {
				
		}
		else if (this->piece == "Knight") {
			for (int i = 0; i < this->moves; i++) {
				if (int(this->kMouse.x / this->mousePosMod) == this->moveset[0][i] and int(this->kMouse.y / this->mousePosMod) == this->moveset[1][i]) {
					this->doStep = true;
					this->dir = i;
				}
			}
		}
		else if (this->piece == "Rook") {
				
		}
		else if (this->piece == "Pawn") {
			if (int(this->kMouse.x / this->mousePosMod) == this->moveset[0][0] and int(this->kMouse.y / this->mousePosMod) == this->moveset[1][0]) {
				this->doStep = true;
				this->dir = 0;
			}
			else if (int(this->kMouse.x / this->mousePosMod) == this->moveset[0][1] and int(this->kMouse.y / this->mousePosMod) == this->moveset[1][1]) {
				this->doStep = true;
				this->dir = 1;
			}
			else if (int(this->kMouse.x / this->mousePosMod) == this->moveset[0][2] and int(this->kMouse.y / this->mousePosMod) == this->moveset[1][2]) {
				this->doStep = true;
				this->dir = 2;
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
