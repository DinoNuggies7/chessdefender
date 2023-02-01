#include "game.h"

int main() {

	// Start Game
	Game game;

	while (game.running()) {
		// Update the Game
		game.update();

		// Render the Game
		game.render();
	}

	return 0;
}
