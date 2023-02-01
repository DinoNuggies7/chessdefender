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
	~Entity();

	void init(std::string entity);
	void update(sf::RenderWindow& _window, bool& _focused, float& _dt, std::vector<std::vector<int>>& _mapLayerCollision);
	void render(sf::RenderWindow& _window);

	bool doStep, turn, inCheck;
	int initiative, dir, dirLengthX, dirLengthY, moves;
	std::string team, piece;
	float x, y, offsetX, offsetY;
	std::vector<int> moveset[2];
	std::vector<int> canTake[2];
protected:
	void physics(float _dt);
	virtual void movement(float _dt);
	void collision(std::vector<std::vector<int>>& _mapLayerCollision);

	void initSprite(std::string _file);
	virtual void setAnimation(std::string _animation);
	void clipTexture(int _x, int _y, int _w, int _h, bool _flip);

	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;
	std::vector<sf::RectangleShape> movesetHighlight;
};

#endif
