#include "game.h"

// ==================================
//  		Public Functions
// ==================================

Game::Game() {
	// Randomizing seed for virtually truely random numbers
	std::srand(std::time(NULL));

	// Initializing some variables
	this->delta = 0;
	this->dt = 0;
	this->gameOver = false;
	this->step = 0;

	this->level.currentRoom = 0;
	this->level.playerID = -1;

	// Create the Window
	this->initWindow(Global::START_FULLSCREEN);

	// Load the first level
	this->level.generateRoom(-1, -1, 1, -1, 1);	// Room 0
	this->level.generateRoom(0, 2, 4, 3, 0);	// Room 1
	this->level.generateRoom(-1, -1, -1, 1, 0);	// Room 2
	this->level.generateRoom(-1, 1, -1, -1, 0);	// Room 3
	this->level.generateRoom(1, -1, 5, -1, 0);	// Room 4
	this->level.generateRoom(4, -1, -1, -1, 0);	// Room 5
	this->level.init(this->level.currentRoom, this->level.room[1][this->level.currentRoom], this->level.room[2][this->level.currentRoom], this->level.room[3][this->level.currentRoom], this->level.room[4][this->level.currentRoom]);
}

Game::~Game() {
	// Freeing up memory because pointers
	for (int i = 0; i < this->level.entity.size(); i++) {
		delete this->level.entity[i];
		printf("Deleted Entity, ID: %d\n", i);
	}
}

bool Game::running() {
	return this->window.isOpen(); // Just for neatness
}

void Game::update() {
	// Running default functions
	this->pollEvents();
	this->updateDelta();

	// Update `Level::playerID` with the index of the player entity
	this->level.playerID = -1;
	for (int i = 0; i < this->level.entity.size(); i++) {
		if (this->level.entity[i]->team == "Player") {
			this->level.playerID = i;
		}
	}
	
	// If the player dies, game over
	if (this->level.playerID == -1)
		this->gameOver = true;

	// Handle Leaving Rooms and Opening Doors
	if (!this->gameOver) {this->handleLevel();}

	// Starting all the entities turns over when all their turns are over
	if (this->step > 20)
		this->step = 0;

	// Incrementing `step` if no entities have an initiative matching it
	if (this->entityCounter == this->level.entity.size())
		this->step++;


	this->entityCounter = 0;
	for (int i = 0; i < this->level.entity.size(); i++) {
		// Call the update function for every entity
		this->level.entity[i]->update(this->window, this->focused, this->dt, this->level.mapLayerCollision);

		// Handle Entity Movement and Collisions
		this->handleEntityMovement(i);

		// Resetting the entity's chosen direction
		this->level.entity[i]->dir = -1;

		// Making sure one entity moves at a time
		if (this->step == this->level.entity[i]->initiative)
			this->level.entity[i]->turn = true;
		else {
			this->level.entity[i]->turn = false;
			this->entityCounter++;
		}
		if (this->level.entity[i]->doStep) {
			this->step++;
			this->level.entity[i]->doStep = false;
			this->level.entity[i]->turn = false;
		}
	}

	for (static bool FULLSCREEN_FIRST = Global::START_FULLSCREEN; FULLSCREEN_FIRST; FULLSCREEN_FIRST = false)
		this->level.entity[this->level.playerID]->fullscreen = float(this->videoMode.width) / Global::WIDTH;
}

void Game::render() {
	// Refreshing the screen
	this->window.clear(sf::Color::Black);

	// Rendering bottom map layers
	this->level.render(this->window, 0);

	//Moving the entities around in the vector so that the ones with highest Y value get displayed last (I have no idea how this works :P)
	std::sort(this->level.entity.begin(), this->level.entity.end(), [] (const auto& lhs, const auto& rhs) {return lhs->y < rhs->y;});

	// Rendering all the entities
	for (int i = 0; i < this->level.entity.size(); i++) {
		this->level.entity[i]->render(this->window);
	}

	// Rendering top map layers (for ceilings, etc.)
	this->level.render(this->window, 1);

	// Game Over Screen
	if (this->gameOver) {
		for (static bool _first = true; _first; _first = false) {
			int _textOffset = this->videoMode.width / Global::WIDTH;
			this->print("You Died", this->videoMode.width / 2 - 46 * _textOffset, this->videoMode.height / 2 - 20 * _textOffset, 18 * _textOffset, 3 * _textOffset, sf::Color(200, 20, 30, 255));
		}
	}

	// Rendering all the stored text
	if (this->textLayer.size() > 0) {
		this->window.setView(this->window.getDefaultView());
		for (int i = 0; i < this->textLayer.size(); i++) {
			this->window.draw(this->textLayer[i]);
		}
		this->window.setView(this->view);
	}

	this->window.display();
}

// ===================================
//  		Private Functions
// ===================================

// Creates a new sf::RenderWindow, creates a view 3 times smaller and applys it along with other window settings, then finally initializes the global font
void Game::initWindow(bool _fullscreen) {
	this->fullscreen = _fullscreen;
	this->view.setSize(Global::WIDTH, Global::HEIGHT);
	this->view.setCenter(Global::WIDTH / 2, Global::HEIGHT / 2);
	if (this->fullscreen) {
		this->videoMode = sf::VideoMode(this->videoMode.getDesktopMode().width, this->videoMode.getDesktopMode().height);
		this->window.create(this->videoMode, "Chess Defender", sf::Style::Titlebar | sf::Style::Close | sf::Style::Fullscreen);
	}
	else {
		this->videoMode = sf::VideoMode(Global::WIN_WIDTH, Global::WIN_HEIGHT);
		this->window.create(this->videoMode, "Chess Defender", sf::Style::Titlebar | sf::Style::Close);
	}
	this->window.setView(this->view);
	//this->window.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width / 2 - Global::WIN_WIDTH / 2, sf::VideoMode::getDesktopMode().height / 2 - Global::WIN_HEIGHT / 2));
	this->window.setKeyRepeatEnabled(false);
	this->font.loadFromFile("assets/Ravenna.ttf");
}

void Game::handleLevel() {
	// If all enemies are dead, then clear the room and open all the doors that lead to another room
	int _nonDoorEntities = 0;
	for (int i = 0; i < this->level.entity.size(); i++) {
		if (this->level.entity[i]->team != "Object")
			_nonDoorEntities++;
	}
	if (_nonDoorEntities == 1 and !this->gameOver) {
		if (this->level.room[0][this->level.currentRoom] == 0) {
			printf("All enemies defeated, opening the doors and clearing the room\n");
			this->level.initDoors(this->level.room[1][this->level.currentRoom], this->level.room[2][this->level.currentRoom], this->level.room[3][this->level.currentRoom], this->level.room[4][this->level.currentRoom]);
			this->level.room[0][this->level.currentRoom] = 1;
		}
	}

	// If the player leaves the room, go to the next room depending on where the player left
	if (this->level.entity[this->level.playerID]->y < 1) { // Up
		printf("Loaded room above, y: %f\n", this->level.entity[this->level.playerID]->y);
		this->level.entity[this->level.playerID]->y = 14;
		this->level.currentRoom = this->level.room[1][this->level.currentRoom];
		if (this->level.room[0][this->level.currentRoom] == 1)
			this->level.init(this->level.currentRoom, this->level.room[1][this->level.currentRoom], this->level.room[2][this->level.currentRoom], this->level.room[3][this->level.currentRoom], this->level.room[4][this->level.currentRoom]);
		else
			this->level.init(this->level.currentRoom, -1 ,-1 ,-1, -1);
	}
	else if (this->level.entity[this->level.playerID]->x > 28) { // Right
		printf("Loaded room to the right, x: %f\n", this->level.entity[this->level.playerID]->x);
		this->level.entity[this->level.playerID]->x = 1;
		this->level.currentRoom = this->level.room[2][this->level.currentRoom];
		if (this->level.room[0][this->level.currentRoom] == 1)
			this->level.init(this->level.currentRoom, this->level.room[1][this->level.currentRoom], this->level.room[2][this->level.currentRoom], this->level.room[3][this->level.currentRoom], this->level.room[4][this->level.currentRoom]);
		else
			this->level.init(this->level.currentRoom, -1 ,-1 ,-1, -1);
	}
	else if (this->level.entity[this->level.playerID]->y > 14) { // Down
		printf("Loaded room below, y: %f\n", this->level.entity[this->level.playerID]->y);
		this->level.entity[this->level.playerID]->y = 1;
		this->level.currentRoom = this->level.room[3][this->level.currentRoom];
		if (this->level.room[0][this->level.currentRoom] == 1)
			this->level.init(this->level.currentRoom, this->level.room[1][this->level.currentRoom], this->level.room[2][this->level.currentRoom], this->level.room[3][this->level.currentRoom], this->level.room[4][this->level.currentRoom]);
		else
			this->level.init(this->level.currentRoom, -1 ,-1 ,-1, -1);
	}
	else if (this->level.entity[this->level.playerID]->x < 1) { // Left
		printf("Loaded room to the left, x: %f\n", this->level.entity[this->level.playerID]->x);
		this->level.entity[this->level.playerID]->x = 28;
		this->level.currentRoom = this->level.room[4][this->level.currentRoom];
		if (this->level.room[0][this->level.currentRoom] == 1)
			this->level.init(this->level.currentRoom, this->level.room[1][this->level.currentRoom], this->level.room[2][this->level.currentRoom], this->level.room[3][this->level.currentRoom], this->level.room[4][this->level.currentRoom]);
		else
			this->level.init(this->level.currentRoom, -1 ,-1 ,-1, -1);
	}
}

void Game::handleEntityMovement(int& _i) {
	int _offsetOffset = this->videoMode.width / Global::WIDTH;
	float _moveSpeed = 2 * _offsetOffset;

	// Deleting an Entity if it gets out of bounds (shouldn't be happening but it is anyways)
	if (this->level.entity[_i]->x < 0 and this->level.entity[_i]->y < 0 or this->level.entity[_i]->x > 29 and this->level.entity[_i]->y > 15)
		this->level.entity.erase(this->level.entity.begin()+_i);

	// Checking for pieces around this one and if there are then set Entity::canTake[][] to wherever this piece is
	this->level.entity[_i]->canTake[0].clear();
	this->level.entity[_i]->canTake[1].clear();
	for (int t = 0; t < this->level.entity[_i]->moves; t++) {
		for (int j = 0; j < this->level.entity.size(); j++) {
			if (this->level.entity[_i]->moveset[0][t] == this->level.entity[j]->x and this->level.entity[_i]->moveset[1][t] == this->level.entity[j]->y and this->level.entity[_i]->team != this->level.entity[j]->team and this->level.entity[j]->team != "Object") {
				this->level.entity[_i]->canTake[0].push_back(this->level.entity[_i]->moveset[0][t]);
				this->level.entity[_i]->canTake[1].push_back(this->level.entity[_i]->moveset[1][t]);
			}
			else {
				this->level.entity[_i]->canTake[0].push_back(0);
				this->level.entity[_i]->canTake[1].push_back(0);
			}
		}
	}

	// Handling chosen Entity Movement
	if (this->level.entity[_i]->dir != -1) {
		if (this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir] < this->level.entity[_i]->x and this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir] < this->level.entity[_i]->y) {
			// Smoothly moving the piece Up Left
			while (this->level.entity[_i]->offsetY > 16 * (this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir] - this->level.entity[_i]->y)) {
				this->updateDelta();
				this->level.entity[_i]->offsetX -= _moveSpeed * this->dt;
				this->level.entity[_i]->offsetY -= _moveSpeed * this->dt;
				this->render();
			}
		}
		else if (this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir] < this->level.entity[_i]->y and this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir] == this->level.entity[_i]->x) {
			// Smoothly moving the piece Up
			while (this->level.entity[_i]->offsetY > 16 * (this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir] - this->level.entity[_i]->y)) {
				this->updateDelta();
				this->level.entity[_i]->offsetY -= _moveSpeed * this->dt;
				this->render();
			}
		}
		else if (this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir] > this->level.entity[_i]->x and this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir] < this->level.entity[_i]->y) {
			// Smoothly moving the piece Up Right
			while (this->level.entity[_i]->offsetX < 16 * (this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir] - this->level.entity[_i]->x)) {
				this->updateDelta();
				this->level.entity[_i]->offsetX += _moveSpeed * this->dt;
				this->level.entity[_i]->offsetY -= _moveSpeed * this->dt;
				this->render();
			}
		}
		else if (this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir] > this->level.entity[_i]->x and this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir] == this->level.entity[_i]->y) {
			// Smoothly moving the piece Right
			while (this->level.entity[_i]->offsetX < 16 * (this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir] - this->level.entity[_i]->x)) {
				this->updateDelta();
				this->level.entity[_i]->offsetX += _moveSpeed * this->dt;
				this->render();
			}
		}
		else if (this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir] > this->level.entity[_i]->x and this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir] > this->level.entity[_i]->y) {
			// Smoothly moving the piece Down Right
			while (this->level.entity[_i]->offsetY < 16 * (this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir] - this->level.entity[_i]->y)) {
				this->updateDelta();
				this->level.entity[_i]->offsetX += _moveSpeed * this->dt;
				this->level.entity[_i]->offsetY += _moveSpeed * this->dt;
				this->render();
			}
		}
		else if (this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir] > this->level.entity[_i]->y and this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir] == this->level.entity[_i]->x) {
			// Smoothly moving the piece Down
			while (this->level.entity[_i]->offsetY < 16 * (this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir] - this->level.entity[_i]->y)) {
				this->updateDelta();
				this->level.entity[_i]->offsetY += _moveSpeed * this->dt;
				this->render();
			}
		}
		else if (this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir] < this->level.entity[_i]->x and this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir] > this->level.entity[_i]->y) {
			// Smoothly moving the piece Down Left
			while (this->level.entity[_i]->offsetX > 16 * (this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir] - this->level.entity[_i]->x)) {
				this->updateDelta();
				this->level.entity[_i]->offsetX -= _moveSpeed * this->dt;
				this->level.entity[_i]->offsetY += _moveSpeed * this->dt;
				this->render();
			}
		}
		else if (this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir] < this->level.entity[_i]->x and this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir] == this->level.entity[_i]->y) {
			// Smoothly moving the piece Left
			while (this->level.entity[_i]->offsetX > 16 * (this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir] - this->level.entity[_i]->x)) {
				this->updateDelta();
				this->level.entity[_i]->offsetX -= _moveSpeed * this->dt;
				this->render();
			}
		}

		this->level.entity[_i]->x = this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir];
		this->level.entity[_i]->y = this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir];
		this->level.entity[_i]->offsetX = 0;
		this->level.entity[_i]->offsetY = 0;

		// Capturing a piece if the moving piece moves on top of said piece
		for (int j = 0; j < this->level.entity.size(); j++) {
			if (_i != j and this->level.entity[_i]->team != this->level.entity[j]->team and this->level.entity[j]->team != "Object") {
				if (this->level.entity[_i]->x == this->level.entity[j]->x and this->level.entity[_i]->y == this->level.entity[j]->y)
					this->level.entity.erase(this->level.entity.begin()+j);
			}
		}
	}

	// Promoting Pawn if it gets to the end of the room
	if (this->level.entity[_i]->piece == "Pawn") {
		if (this->level.entity[_i]->y < 2) {
			int _rand = std::rand() % 3;
			switch (_rand) {
				case 0:
					this->level.entity[_i]->init("Bishop");
					break;
				case 1:
					this->level.entity[_i]->init("Knight");
					break;
				case 2:
					this->level.entity[_i]->init("Rook");
					break;
			}
		}
	}

	/*
	// The King piece's moves
	if (this->level.entity[_i]->piece == "King") {
		if (this->level.entity[_i]->dir != -1) {
			if (this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir] < this->level.entity[_i]->x and this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir] < this->level.entity[_i]->y) {
				// Smoothly moving the piece Up Left
				while (this->level.entity[_i]->offsetY > 16 * (this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir] - this->level.entity[_i]->y)) {
					this->updateDelta();
					this->level.entity[_i]->offsetX -= _moveSpeed * this->dt;
					this->level.entity[_i]->offsetY -= _moveSpeed * this->dt;
					this->render();
				}
			}

			else if (this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir] < this->level.entity[_i]->y) {
				// Smoothly moving the piece Up
				while (this->level.entity[_i]->offsetY > 16 * (this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir] - this->level.entity[_i]->y)) {
					this->updateDelta();
					this->level.entity[_i]->offsetY -= _moveSpeed * this->dt;
					this->render();
				}
			}

			else if (this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir] > this->level.entity[_i]->x and this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir] < this->level.entity[_i]->y) {
				// Smoothly moving the piece Up Right
				while (this->level.entity[_i]->offsetX < 16 * (this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir] - this->level.entity[_i]->x)) {
					this->updateDelta();
					this->level.entity[_i]->offsetX += _moveSpeed * this->dt;
					this->level.entity[_i]->offsetY -= _moveSpeed * this->dt;
					this->render();
				}
			}

			else if (this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir] > this->level.entity[_i]->x) {
				// Smoothly moving the piece Right
				while (this->level.entity[_i]->offsetX < 16 * (this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir] - this->level.entity[_i]->x)) {
					this->updateDelta();
					this->level.entity[_i]->offsetX += _moveSpeed * this->dt;
					this->render();
				}
			}

			else if (this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir] > this->level.entity[_i]->x and this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir] > this->level.entity[_i]->y) {
				// Smoothly moving the piece Down Right
				while (this->level.entity[_i]->offsetY < 16 * (this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir] - this->level.entity[_i]->y)) {
					this->updateDelta();
					this->level.entity[_i]->offsetX += _moveSpeed * this->dt;
					this->level.entity[_i]->offsetY += _moveSpeed * this->dt;
					this->render();
				}
			}

			else if (this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir] > this->level.entity[_i]->y) {
				// Smoothly moving the piece Down
				while (this->level.entity[_i]->offsetY < 16 * (this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir] - this->level.entity[_i]->y)) {
					this->updateDelta();
					this->level.entity[_i]->offsetY += _moveSpeed * this->dt;
					this->render();
				}
			}

			else if (this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir] < this->level.entity[_i]->x and this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir] > this->level.entity[_i]->y) {
				// Smoothly moving the piece Down Left
				while (this->level.entity[_i]->offsetX > 16 * (this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir] - this->level.entity[_i]->x)) {
					this->updateDelta();
					this->level.entity[_i]->offsetX -= _moveSpeed * this->dt;
					this->level.entity[_i]->offsetY += _moveSpeed * this->dt;
					this->render();
				}
			}

			else if (this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir] < this->level.entity[_i]->x) {
				// Smoothly moving the piece Left
				while (this->level.entity[_i]->offsetX > 16 * (this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir] - this->level.entity[_i]->x)) {
					this->updateDelta();
					this->level.entity[_i]->offsetX -= _moveSpeed * this->dt;
					this->render();
				}
			}
			this->level.entity[_i]->x = this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir];
			this->level.entity[_i]->y = this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir];
			this->level.entity[_i]->offsetX = 0;
			this->level.entity[_i]->offsetY = 0;
		}

		// Capturing a piece if the moving piece moves on top of said piece
		for (int j = 0; j < this->level.entity.size(); j++) {
			if (_i != j and this->level.entity[_i]->team != this->level.entity[j]->team and this->level.entity[j]->team != "Object") {
				if (this->level.entity[_i]->x == this->level.entity[j]->x and this->level.entity[_i]->y == this->level.entity[j]->y)
					this->level.entity.erase(this->level.entity.begin()+j);
			}
		}
	}

	// The Queen piece's moves
	else if (this->level.entity[_i]->piece == "Queen") {

	}

	else if (this->level.entity[_i]->piece == "Bishop") {
		// Checking for pieces around this one and if there are then set Entity::canTake[][] to wherever this piece is
		this->level.entity[_i]->canTake[0].clear();
		this->level.entity[_i]->canTake[1].clear();
		for (int t = 0; t < this->level.entity[_i]->moves; t++) {
			for (int j = 0; j < this->level.entity.size(); j++) {
				if (this->level.entity[_i]->moveset[0][t] == this->level.entity[j]->x and this->level.entity[_i]->moveset[1][t] == this->level.entity[j]->y and this->level.entity[j]->team != this->level.entity[_i]->team and this->level.entity[j]->team != "Object") {
					this->level.entity[_i]->canTake[0].push_back(this->level.entity[_i]->moveset[0][t]);
					this->level.entity[_i]->canTake[1].push_back(this->level.entity[_i]->moveset[1][t]);
				}
				else {
					this->level.entity[_i]->canTake[0].push_back(0);
					this->level.entity[_i]->canTake[1].push_back(0);
				}
			}
		}
		// Handling Entity Movement Choice
		if (this->level.entity[_i]->dir != -1) {
			if (this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir] < this->level.entity[_i]->x and this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir] < this->level.entity[_i]->y) {
				// Smoothly moving the piece Up Left
				while (this->level.entity[_i]->offsetY > 16 * (this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir] - this->level.entity[_i]->y)) {
					this->updateDelta();
					this->level.entity[_i]->offsetX -= _moveSpeed * this->dt;
					this->level.entity[_i]->offsetY -= _moveSpeed * this->dt;
					this->render();
				}
			}
			else if (this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir] > this->level.entity[_i]->x and this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir] < this->level.entity[_i]->y) {
				// Smoothly moving the piece Up Right
				while (this->level.entity[_i]->offsetX < 16 * (this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir] - this->level.entity[_i]->x)) {
					this->updateDelta();
					this->level.entity[_i]->offsetX += _moveSpeed * this->dt;
					this->level.entity[_i]->offsetY -= _moveSpeed * this->dt;
					this->render();
				}
			}
			else if (this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir] > this->level.entity[_i]->x and this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir] > this->level.entity[_i]->y) {
				// Smoothly moving the piece Down Right
				while (this->level.entity[_i]->offsetY < 16 * (this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir] - this->level.entity[_i]->y)) {
					this->updateDelta();
					this->level.entity[_i]->offsetX += _moveSpeed * this->dt;
					this->level.entity[_i]->offsetY += _moveSpeed * this->dt;
					this->render();
				}
			}
			else if (this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir] < this->level.entity[_i]->x and this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir] > this->level.entity[_i]->y) {
				// Smoothly moving the piece Down Left
				while (this->level.entity[_i]->offsetX > 16 * (this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir] - this->level.entity[_i]->x)) {
					this->updateDelta();
					this->level.entity[_i]->offsetX -= _moveSpeed * this->dt;
					this->level.entity[_i]->offsetY += _moveSpeed * this->dt;
					this->render();
				}
			}
			this->level.entity[_i]->x = this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir];
			this->level.entity[_i]->y = this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir];
			this->level.entity[_i]->offsetX = 0;
			this->level.entity[_i]->offsetY = 0;
		}

		// Capturing a piece if the moving piece moves on top of said piece
		for (int j = 0; j < this->level.entity.size(); j++) {
			if (_i != j and this->level.entity[_i]->team != this->level.entity[j]->team and this->level.entity[j]->team != "Object") {
				if (this->level.entity[_i]->x == this->level.entity[j]->x and this->level.entity[_i]->y == this->level.entity[j]->y)
					this->level.entity.erase(this->level.entity.begin()+j);
			}
		}
	}
	else if (this->level.entity[_i]->piece == "Knight") {
		// Checking for pieces around this one and if there are then set Entity::canTake[][] to wherever this piece is
		this->level.entity[_i]->canTake[0].clear();
		this->level.entity[_i]->canTake[1].clear();
		for (int t = 0; t < this->level.entity[_i]->moves; t++) {
			for (int j = 0; j < this->level.entity.size(); j++) {
				if (this->level.entity[_i]->moveset[0][t] == this->level.entity[j]->x and this->level.entity[_i]->moveset[1][t] == this->level.entity[j]->y and this->level.entity[j]->team != this->level.entity[_i]->team and this->level.entity[j]->team != "Object") {
					this->level.entity[_i]->canTake[0].push_back(this->level.entity[_i]->moveset[0][t]);
					this->level.entity[_i]->canTake[1].push_back(this->level.entity[_i]->moveset[1][t]);
				}
				else {
					this->level.entity[_i]->canTake[0].push_back(0);
					this->level.entity[_i]->canTake[1].push_back(0);
				}
			}
		}

		// Handling Entity Movement Choice
		switch (this->level.entity[_i]->dir) {
			case 0:
				// Smoothly moving the piece Up Left
				while (this->level.entity[_i]->offsetY > 16 * (this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir] - this->level.entity[_i]->y)) {
					this->updateDelta();
					this->level.entity[_i]->offsetY -= _moveSpeed * this->dt;
					this->render();
				}
				while (this->level.entity[_i]->offsetX > 16 * (this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir] - this->level.entity[_i]->x)) {
					this->updateDelta();
					this->level.entity[_i]->offsetX -= _moveSpeed * this->dt;
					this->render();
				}
				break;
			case 1:
				// Smoothly moving the piece Up Right
				while (this->level.entity[_i]->offsetY > 16 * (this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir] - this->level.entity[_i]->y)) {
					this->updateDelta();
					this->level.entity[_i]->offsetY -= _moveSpeed * this->dt;
					this->render();
				}
				while (this->level.entity[_i]->offsetX < 16 * (this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir] - this->level.entity[_i]->x)) {
					this->updateDelta();
					this->level.entity[_i]->offsetX += _moveSpeed * this->dt;
					this->render();
				}
				break;
			case 2:
				// Smoothly moving the piece Right Up
				while (this->level.entity[_i]->offsetX < 16 * (this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir] - this->level.entity[_i]->x)) {
					this->updateDelta();
					this->level.entity[_i]->offsetX += _moveSpeed * this->dt;
					this->render();
				}
				while (this->level.entity[_i]->offsetY > 16 * (this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir] - this->level.entity[_i]->y)) {
					this->updateDelta();
					this->level.entity[_i]->offsetY -= _moveSpeed * this->dt;
					this->render();
				}
				break;
			case 3:
				// Smoothly moving the piece Right Down
				while (this->level.entity[_i]->offsetX < 16 * (this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir] - this->level.entity[_i]->x)) {
					this->updateDelta();
					this->level.entity[_i]->offsetX += _moveSpeed * this->dt;
					this->render();
				}
				while (this->level.entity[_i]->offsetY < 16 * (this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir] - this->level.entity[_i]->y)) {
					this->updateDelta();
					this->level.entity[_i]->offsetY += _moveSpeed * this->dt;
					this->render();
				}
				break;
			case 4:
				// Smoothly moving the piece Down Right
				while (this->level.entity[_i]->offsetY < 16 * (this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir] - this->level.entity[_i]->y)) {
					this->updateDelta();
					this->level.entity[_i]->offsetY += _moveSpeed * this->dt;
					this->render();
				}
				while (this->level.entity[_i]->offsetX < 16 * (this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir] - this->level.entity[_i]->x)) {
					this->updateDelta();
					this->level.entity[_i]->offsetX += _moveSpeed * this->dt;
					this->render();
				}
				break;
			case 5:
				// Smoothly moving the piece Down Left
				while (this->level.entity[_i]->offsetY < 16 * (this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir] - this->level.entity[_i]->y)) {
					this->updateDelta();
					this->level.entity[_i]->offsetY += _moveSpeed * this->dt;
					this->render();
				}
				while (this->level.entity[_i]->offsetX > 16 * (this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir] - this->level.entity[_i]->x)) {
					this->updateDelta();
					this->level.entity[_i]->offsetX -= _moveSpeed * this->dt;
					this->render();
				}
				break;
			case 6:
				// Smoothly moving the piece Left Down
				while (this->level.entity[_i]->offsetX > 16 * (this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir] - this->level.entity[_i]->x)) {
					this->updateDelta();
					this->level.entity[_i]->offsetX -= _moveSpeed * this->dt;
					this->render();
				}
				while (this->level.entity[_i]->offsetY < 16 * (this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir] - this->level.entity[_i]->y)) {
					this->updateDelta();
					this->level.entity[_i]->offsetY += _moveSpeed * this->dt;
					this->render();
				}
				break;
			case 7:
				// Smoothly moving the piece Left Up
				while (this->level.entity[_i]->offsetX > 16 * (this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir] - this->level.entity[_i]->x)) {
					this->updateDelta();
					this->level.entity[_i]->offsetX -= _moveSpeed * this->dt;
					this->render();
				}
				while (this->level.entity[_i]->offsetY > 16 * (this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir] - this->level.entity[_i]->y)) {
					this->updateDelta();
					this->level.entity[_i]->offsetY -= _moveSpeed * this->dt;
					this->render();
				}
				break;
		}
		if (this->level.entity[_i]->dir != -1) {
			this->level.entity[_i]->x = this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir];
			this->level.entity[_i]->y = this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir];
			this->level.entity[_i]->offsetX = 0;
			this->level.entity[_i]->offsetY = 0;
		}

		// Capturing a piece if the moving piece moves on top of said piece
		for (int j = 0; j < this->level.entity.size(); j++) {
			if (_i != j and this->level.entity[_i]->team != this->level.entity[j]->team and this->level.entity[j]->team != "Object") {
				if (this->level.entity[_i]->x == this->level.entity[j]->x and this->level.entity[_i]->y == this->level.entity[j]->y)
					this->level.entity.erase(this->level.entity.begin()+j);
			}
		}
	}
	else if (this->level.entity[_i]->piece == "Rook") {
		// Checking for pieces around this one and if there are then set Entity::canTake[][] to wherever this piece is
		this->level.entity[_i]->canTake[0].clear();
		this->level.entity[_i]->canTake[1].clear();
		for (int t = 0; t < this->level.entity[_i]->moves; t++) {
			for (int j = 0; j < this->level.entity.size(); j++) {
				if (this->level.entity[_i]->moveset[0][t] == this->level.entity[j]->x and this->level.entity[_i]->moveset[1][t] == this->level.entity[j]->y and this->level.entity[j]->team != this->level.entity[_i]->team and this->level.entity[j]->team != "Object") {
					this->level.entity[_i]->canTake[0].push_back(this->level.entity[_i]->moveset[0][t]);
					this->level.entity[_i]->canTake[1].push_back(this->level.entity[_i]->moveset[1][t]);
				}
				else {
					this->level.entity[_i]->canTake[0].push_back(0);
					this->level.entity[_i]->canTake[1].push_back(0);
				}
			}
		}
		// Handling Entity Movement Choice
		if (this->level.entity[_i]->dir != -1) {
			if (this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir] < this->level.entity[_i]->y) {
				// Smoothly moving the piece Up
				while (this->level.entity[_i]->offsetY > 16 * (this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir] - this->level.entity[_i]->y)) {
					this->updateDelta();
					this->level.entity[_i]->offsetY -= _moveSpeed * this->dt;
					this->render();
				}
			}
			else if (this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir] > this->level.entity[_i]->x) {
				// Smoothly moving the piece Right
				while (this->level.entity[_i]->offsetX < 16 * (this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir] - this->level.entity[_i]->x)) {
					this->updateDelta();
					this->level.entity[_i]->offsetX += _moveSpeed * this->dt;
					this->render();
				}
			}
			else if (this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir] > this->level.entity[_i]->y) {
				// Smoothly moving the piece Down
				while (this->level.entity[_i]->offsetY < 16 * (this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir] - this->level.entity[_i]->y)) {
					this->updateDelta();
					this->level.entity[_i]->offsetY += _moveSpeed * this->dt;
					this->render();
				}
			}
			else if (this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir] < this->level.entity[_i]->x) {
				// Smoothly moving the piece Left
				while (this->level.entity[_i]->offsetX > 16 * (this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir] - this->level.entity[_i]->x)) {
					this->updateDelta();
					this->level.entity[_i]->offsetX -= _moveSpeed * this->dt;
					this->render();
				}
			}
			this->level.entity[_i]->x = this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir];
			this->level.entity[_i]->y = this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir];
			this->level.entity[_i]->offsetX = 0;
			this->level.entity[_i]->offsetY = 0;
		}

		// Capturing a piece if the moving piece moves on top of said piece
		for (int j = 0; j < this->level.entity.size(); j++) {
			if (_i != j and this->level.entity[_i]->team != this->level.entity[j]->team and this->level.entity[j]->team != "Object") {
				if (this->level.entity[_i]->x == this->level.entity[j]->x and this->level.entity[_i]->y == this->level.entity[j]->y)
					this->level.entity.erase(this->level.entity.begin()+j);
			}
		}
	}

	else if (this->level.entity[_i]->piece == "Pawn") {
		// Checking for pieces around this one and if there are then set Entity::canTake[][] to wherever this piece is
		this->level.entity[_i]->canTake[0].clear();
		this->level.entity[_i]->canTake[1].clear();
		for (int t = 0; t < this->level.entity[_i]->moves; t++) {
			for (int j = 0; j < this->level.entity.size(); j++) {
				if (this->level.entity[_i]->moveset[0][t] == this->level.entity[j]->x and this->level.entity[_i]->moveset[1][t] == this->level.entity[j]->y and this->level.entity[j]->team != this->level.entity[_i]->team and this->level.entity[j]->team != "Object") {
					this->level.entity[_i]->canTake[0].push_back(this->level.entity[_i]->moveset[0][t]);
					this->level.entity[_i]->canTake[1].push_back(this->level.entity[_i]->moveset[1][t]);
				}
				else {
					this->level.entity[_i]->canTake[0].push_back(0);
					this->level.entity[_i]->canTake[1].push_back(0);
				}
			}
		}

		// Handling Entity movement choices
		if (this->level.entity[_i]->dir != -1) {
			if (this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir] < this->level.entity[_i]->y and this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir] < this->level.entity[_i]->x) {
				// Smoothly moving the piece Up Left
				while (this->level.entity[_i]->offsetX > -16 or this->level.entity[_i]->offsetY > -16) {
					this->updateDelta();
					this->level.entity[_i]->offsetX -= _moveSpeed * this->dt;
					this->level.entity[_i]->offsetY -= _moveSpeed * this->dt;
					this->render();
				}
			}
			else if (this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir] < this->level.entity[_i]->y and this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir] == this->level.entity[_i]->x) {
				// Smoothly moving the piece Up
				while (this->level.entity[_i]->offsetY > -16) {
					this->updateDelta();
					this->level.entity[_i]->offsetY -= _moveSpeed * this->dt;
					this->render();
				}
			}
			else if (this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir] < this->level.entity[_i]->y and this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir] > this->level.entity[_i]->x) {
				// Smoothly moving the piece Up Right
				while (this->level.entity[_i]->offsetX < 16 or this->level.entity[_i]->offsetY > -16) {
					this->updateDelta();
					this->level.entity[_i]->offsetX += _moveSpeed * this->dt;
					this->level.entity[_i]->offsetY -= _moveSpeed * this->dt;
					this->render();
				}
			}
			this->level.entity[_i]->x = this->level.entity[_i]->moveset[0][this->level.entity[_i]->dir];
			this->level.entity[_i]->y = this->level.entity[_i]->moveset[1][this->level.entity[_i]->dir];
			this->level.entity[_i]->offsetX = 0;
			this->level.entity[_i]->offsetY = 0;
		}

		// Capturing a piece if the moving piece moves on top of said piece
		for (int j = 0; j < this->level.entity.size(); j++) {
			if (_i != j and this->level.entity[_i]->team != this->level.entity[j]->team and this->level.entity[j]->team != "Object") {
				if (this->level.entity[_i]->x == this->level.entity[j]->x and this->level.entity[_i]->y == this->level.entity[j]->y)
					this->level.entity.erase(this->level.entity.begin()+j);
			}
		}
		
		// Promoting Pawn if it gets to the end of the room
		if (this->level.entity[_i]->y < 2) {
			int _rand = std::rand() % 3;
			switch (_rand) {
				case 0:
					this->level.entity[_i]->init("Bishop");
					break;
				case 1:
					this->level.entity[_i]->init("Knight");
					break;
				case 2:
					this->level.entity[_i]->init("Rook");
					break;
			}
		}
	}
	*/
}

// Updated `dt` every frame, just in case I need frame independant animations
void Game::updateDelta() {
	this->delta = this->clock.restart().asSeconds();
	this->dt = delta * Global::VIRTUAL_FRAMERATE;
}

// The regular SFML window events
void Game::pollEvents() {
	while (this->window.pollEvent(this->ev)) {
		switch (this->ev.type) {
			case sf::Event::GainedFocus:
				this->focused = true;
				break;
			case sf::Event::LostFocus:
				this->focused = false;
				break;
			case sf::Event::Closed:
				this->window.close();
				break;
			case sf::Event::KeyPressed:
				if (this->ev.key.code == sf::Keyboard::Escape)
					this->window.close();
				if (this->ev.key.code == sf::Keyboard::F11) {
					if (this->fullscreen) {
						this->initWindow(false);
						this->level.entity[this->level.playerID]->fullscreen = 0;
					}
					else {
						this->initWindow(true);
						this->level.entity[this->level.playerID]->fullscreen = float(this->videoMode.width) / Global::WIDTH;
					}
				}
				break;
		}
	}
}

// Adds a text object to the text layer for displaying later (ONLY CALL ONCE)
void Game::print(std::string _string, int _x, int _y, int _size, int _shadowOffset, sf::Color _color) {
	sf::Text _shadowText;
	_shadowText.setFont(this->font);
	_shadowText.setString(_string);
	_shadowText.setPosition(_x - _shadowOffset, _y - _shadowOffset);
	_shadowText.setCharacterSize(_size);
	_shadowText.setFillColor(sf::Color::Black);
	this->textLayer.push_back(_shadowText);
	sf::Text _text;
	_text.setFont(this->font);
	_text.setString(_string);
	_text.setPosition(_x, _y);
	_text.setCharacterSize(_size);
	_text.setFillColor(_color);
	this->textLayer.push_back(_text);
}
