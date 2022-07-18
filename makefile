all:
	ccache g++ src/main.cpp src/game.cpp src/input.cpp src/entity.cpp -o game -lsfml-graphics -lsfml-window -lsfml-system
	./game