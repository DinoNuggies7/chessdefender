#include <SFML/Graphics.hpp>

class Entity {
public:
	void init(const char* _spritePath);
	void update(float _dt);
	void render(sf::RenderWindow* _window);
protected:
	void initSprite(const char* _spritePath);
	void setAnimation();

	float x, y, vx, vy;

	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;
};