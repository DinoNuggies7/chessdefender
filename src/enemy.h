#ifndef ENEMY_H
#define ENEMY_H

#include "entity.h"

class Enemy : public Entity {
public:
	Enemy();
protected:
	void movement(float _dt);
	void initSprite();
	void setAnimation(const char* _animation);
};

#endif