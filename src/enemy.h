#ifndef ENEMY_H
#define ENEMY_H

#include "entity.h"

class Enemy : public Entity {
public:
	Enemy();
private:
	void movement(float _dt);
	void setAnimation(std::string _animation);
};

#endif