#ifndef ENTITY_H
#define ENTITY_H

#include "input.h"

#include <SFML/Graphics.hpp>

#include <random>
#include <time.h>

class Entity : public Input {
public:
	void init();
	void update(float _dt);
	void render(sf::RenderWindow* _window);

	bool doStep, turn, inCheck;
	int initiative, dir;
	char* team;
	float x, y, vx, vy;
protected:
	void physics(float _dt);
	virtual void movement(float _dt);
	virtual void collision(float _dt);

	virtual void initSprite();
	virtual void setAnimation(const char* _animation);
	void clipTexture(int _x, int _y, int _w, int _h, bool _flip);

	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;

	int frame;
};

#endif
