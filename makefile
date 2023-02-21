all:
	clear
	mkdir -p build/
	ccache g++ -c src/main.cpp -o build/main.o -O
	ccache g++ -c src/game.cpp -o build/game.o -O
	ccache g++ -c src/level.cpp -o build/level.o -O
	ccache g++ -c src/input.cpp -o build/input.o -O
	ccache g++ -c src/entity.cpp -o build/entity.o -O
	ccache g++ -c src/player.cpp -o build/player.o -O
	ccache g++ -c src/enemy.cpp -o build/enemy.o -O
	ccache g++ -c src/door.cpp -o build/door.o -O
	ccache g++ build/*.o -lsfml-graphics -lsfml-window -lsfml-system -ltmxlite -ltinyxml2 -O
	./a.out
