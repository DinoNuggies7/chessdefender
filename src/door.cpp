#include "door.h"

// ==================================
//  		Public Functions
// ==================================

// Constructor
Door::Door(int _position) {
	this->team = "Object";
	this->initiative = -1;
	this->piece = "Door";
	this->initSprite("assets/mainlevbuild.png");
	if (_position == 0) {
		this->setAnimation("DoorTop");
		this->x = 14;
		this->y = 0;
		this->piece = "DoorTop";
	}
	else if (_position == 1) {
		this->setAnimation("DoorRight");
		this->x = 29;
		this->y = 7;
		this->piece = "DoorRight";
	}
	else if (_position == 2) {
		this->setAnimation("DoorBottom");
		this->x = 14;
		this->y = 15;
		this->piece = "DoorBottom";
	}
	else if (_position == 3) {
		this->setAnimation("DoorLeft");
		this->x = 0;
		this->y = 7;
		this->piece = "DoorLeft";
	}
}


// ===================================
//  		Private Functions
// ===================================

// For changing the Door's sprite
void Door::setAnimation(std::string _animation) {
	if (_animation == "DoorTop")
		this->clipTexture(80, 112, 32, 16, false);
	else if (_animation == "DoorBottom")
		this->clipTexture(96, 48, 32, 16, false);
	else if (_animation == "DoorLeft")
		this->clipTexture(240, 64, 16, 32, false);
	else if (_animation == "DoorRight")
		this->clipTexture(64, 80, 16, 32, false);
}