#ifndef ENTITY_H
#define ENTITY_H

#include "input.h"

#include <SFML/Graphics.hpp>

#include <stdio.h>
#include <vector>
#include <random>
#include <time.h>
#include <string>

class Entity : public Input {
public:
	void init(std::string _piece);
	void update(sf::RenderWindow& _window, float& _dt, std::vector<std::vector<int>>& _mapLayerCollision);
	void render(sf::RenderWindow& _window);

	bool doStep, turn, inCheck, isSelected;
	int initiative, dir, moves;
	std::string team, piece;
	float x, y, vx, vy, offsetX, offsetY;
	std::vector<int> moveset[2];
protected:
	void physics(float _dt);
	virtual void movement(float _dt);
	void collision(std::vector<std::vector<int>>& _mapLayerCollision);

	void initSprite();
	virtual void setAnimation(std::string _animation);
	void setMoveset(std::string _piece);
	void clipTexture(int _x, int _y, int _w, int _h, bool _flip);

	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;
	std::vector<sf::RectangleShape> movesetHighlight;
};

#endif
