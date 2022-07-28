all:
	ccache g++ src/main.cpp src/game.cpp src/level.cpp src/input.cpp src/entity.cpp src/player.cpp src/enemy.cpp -o game -lsfml-graphics -lsfml-window -lsfml-system -ltmxlite -ltinyxml2
	./game

android:
	arm-linux-androideabi-g++ src/main.cpp src/game.cpp src/level.cpp src/input.cpp src/entity.cpp src/player.cpp src/enemy.cpp -o game -I/home/dinonuggies/SFML/SFML/include -L/home/dinonuggies/SFML/SFML/lib -lsfml-graphics -lsfml-window -lsfml-system -ltmxlite -ltinyxml2
