all:
	ccache g++ src/main.cpp src/game.cpp src/input.cpp src/entity.cpp src/player.cpp src/enemy.cpp -o game -lsfml-graphics -lsfml-window -lsfml-system -ltinyxml2
	./game