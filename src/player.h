#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"

class Player : public Entity {
public:
	Player();
private:
	void movement(float _dt);
	void initSprite();
	void setAnimation(const char* _animation);
};

#endif