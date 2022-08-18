#ifndef ENTITY_H
#define ENTITY_H

#include "input.h"

#include <SFML/Graphics.hpp>

#include <stdio.h>
#include <random>
#include <time.h>
#include <string>

class Entity : public Input {
public:
	void init(std::string _piece);
	void update(float _dt);
	void render(sf::RenderWindow*& _window);

	bool doStep, turn, inCheck;
	int initiative, dir;
	std::string team, piece;
	float x, y, vx, vy;
protected:
	void physics(float _dt);
	virtual void movement(float _dt);
	virtual void collision(float _dt);

	void initSprite();
	virtual void setAnimation(std::string _animation);
	void clipTexture(int _x, int _y, int _w, int _h, bool _flip);

	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;
};

#endif
