#include "player.h"

// ==================================
//  		Public Functions
// ==================================

// Constructer
Player::Player() {
	this->team = "Player";
	this->x = 14;
	this->y = 7;
	this->dir = -1;
}

// ===================================
//  		Private Functions
// ===================================

// General Input and Deciding which Move to do
void Player::movement(float _dt) {
	if (this->kMouseL) {
		for (int i = 0; i < this->moves; i++) {
			if (int(this->kMouse.x / this->mousePosMod) == this->moveset[0][i] and int(this->kMouse.y / this->mousePosMod) == this->moveset[1][i]) {
				this->doStep = true;
				this->dir = i;
			}
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
