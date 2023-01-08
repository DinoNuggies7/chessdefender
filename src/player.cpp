#include "player.h"

// ==================================
//  		Public Functions
// ==================================

// Constructer
Player::Player() {
	this->team = "Player";
	this->x = 14;
	this->y = 7;
}

// ===================================
//  		Private Functions
// ===================================

// General Input and Deciding which Move to do
void Player::movement(float _dt) {
	if (this->kMouseLP) {
		for (int i = 0; i < this->moves; i++) {
			if (int(this->kMouse.x / this->mousePosMod) == this->moveset[0][i] and int(this->kMouse.y / this->mousePosMod) == this->moveset[1][i]) {
				this->doStep = true;
				this->dir = i;
			}
		}
	}
	// so I don't have to click a lot at night, which is really loud
	if (this->piece == "King") {
		if (this->kUpP) {
			this->doStep = true;
			this->dir = 1;
		}
		else if (this->kRightP) {
			this->doStep = true;
			this->dir = 3;
		}
		else if (this->kDownP) {
			this->doStep = true;
			this->dir = 5;
		}
		else if (this->kLeftP) {
			this->doStep = true;
			this->dir = 7;
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
