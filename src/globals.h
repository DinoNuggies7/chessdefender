#ifndef GLOBAL_H
#define GLOBAL_H

#include <SFML/Graphics.hpp>

#include <vector>
#include <string>

namespace Global {
	const int WIDTH = 480, HEIGHT = 270, WIN_SCALE = 3, WIN_WIDTH = WIDTH * WIN_SCALE, WIN_HEIGHT = HEIGHT * WIN_SCALE;
	const float FRAMERATE = 60;
	sf::Font font;

	// Adds a text object to the text layer for displaying later (ONLY CALL ONCE)
	void print(std::vector<sf::Text>& _textLayer, std::string _string, int _x, int _y, int _size, sf::Color _color) {
		sf::Text _text;
		_text.setFont(font);
		_text.setString(_string);
		_text.setPosition(_x, _y);
		_text.setCharacterSize(_size);
		_text.setFillColor(_color);
		_textLayer.push_back(_text);
	}
};

#endif