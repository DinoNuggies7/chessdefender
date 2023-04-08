all:
	clear
	mkdir -p build/
	ccache g++ -c src/main.cpp -o build/main.o
	ccache g++ -c src/game.cpp -o build/game.o
	ccache g++ -c src/level.cpp -o build/level.o
	ccache g++ -c src/input.cpp -o build/input.o
	ccache g++ -c src/entity.cpp -o build/entity.o
	ccache g++ -c src/player.cpp -o build/player.o
	ccache g++ -c src/enemy.cpp -o build/enemy.o
	ccache g++ -c src/door.cpp -o build/door.o
	ccache g++ build/*.o -lsfml-graphics -lsfml-window -lsfml-system -ltmxlite -ltinyxml2
	./a.out

clean:
	clear
	rm -r -f build/
	rm -f a.out