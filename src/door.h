#ifndef DOOR_H
#define DOOR_H

#include "entity.h"

class Door : public Entity {
public:
	Door(int _position);
private:
	void setAnimation(std::string _animation);
};

#endif