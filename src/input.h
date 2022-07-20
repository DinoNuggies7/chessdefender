#ifndef INPUT_H
#define INPUT_H

#include <SFML/Window.hpp>

class Input {
protected:
	void input();

	bool kInput, kUp, kDown, kLeft, kRight, kUpP, kDownP, kLeftP, kRightP, kA, kB;
};

#endif