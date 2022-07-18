#include <SFML/Window.hpp>

class Input {
protected:
	void input();

	bool kInput, kUp, kDown, kLeft, kRight, kA, kB;
	int dir;
};