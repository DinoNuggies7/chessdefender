#include <SFML/Graphics.hpp>

#include "input.h"

class Entity : public Input {
public:
	void init(const char* _spritePath);
	void update(float _dt);
	void render(sf::RenderWindow* _window);
protected:
	void physics(float _dt);
	void collision(float _dt);

	void initSprite(const char* _spritePath);
	void setAnimation(const char* _animation);
	void clipTexture(int _x, int _y, int _w, int _h, bool _flip);

	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;

	int frame;
	float x, y, vx, vy;
};