#include "game.h"

// ==================================
//  		Public Functions
// ==================================

Game::Game() {
	// Randomizing seed for virtually truely random numbers
	std::srand(std::time(NULL));

	// Initializing Game class variables
	this->delta = 0;
	this->dt = 0;
	this->gameOver = false;
	this->step = 0;

	// More initialization
	this->initWindow(false);
	this->level.init(0);
}

Game::~Game() {
	// Freeing up memory because pointers
	//delete this->window;
	for (int i = 0; i < this->level.entities; i++) {
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

	// Updating `entities`
	this->level.entities = this->level.entity.size();

	// Starting all the entities turns over when all their turns are over
	if (this->step > 16)
		this->step = 0;

	// Incrementing `step` if no entities have an initiative matching it
	if (this->entityCounter == this->level.entities)
		this->step++;

	// Making sure the camera always follows the player regardless of it's vector position
	this->level.playerID = -1;
	for (int i = 0; i < this->level.entities; i++) {
		if (this->level.entity[i]->team == "Player")
			this->level.playerID = i;
	}
	if (this->level.playerID == -1)
		this->gameOver = true;

	this->entityCounter = 0;
	for (int i = 0; i < this->level.entities; i++) {
		// Call the update function for every entity
		this->level.entity[i]->update(this->window, this->dt, this->level.mapLayerCollision);

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
}

void Game::render() {
	this->window.clear(sf::Color::Black);

	// Rendering bottom map layers
	this->level.render(this->window, 0);

	//Moving the entities around in the vector so that the ones with highest Y value get displayed last (I have no idea how this works :P)
	std::sort(this->level.entity.begin(), this->level.entity.end(), [] (const auto& lhs, const auto& rhs) {return lhs->y < rhs->y;});

	// Rendering all the entities
	for (int i = 0; i < this->level.entities; i++) {
		this->level.entity[i]->render(this->window);
	}

	// Rendering top map layers (for ceilings, etc.)
	this->level.render(this->window, 1);

	// Game Over Screen
	if (this->gameOver) {
		for (static bool _first = true; _first; _first = false)
			this->print("You Died", Global::WIN_WIDTH / 2 - 96, Global::WIN_HEIGHT / 2 - 16, 36, sf::Color(200, 20, 30, 255));
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
		this->videoMode = sf::VideoMode(0, 0);
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

void Game::handleEntityMovement(int& _i) {

	// The King piece's moves
	if (this->level.entity[_i]->piece == "King") {
		switch (this->level.entity[_i]->dir) {
			case 0:		// ================ Up ================
				// Capturing a piece if the moving piece moves on top of said piece
				for (int j = 0; j < this->level.entities; j++) {
					if (_i != j and this->level.entity[_i]->team != this->level.entity[j]->team) {
						if (this->level.entity[_i]->y - 1 == this->level.entity[j]->y and this->level.entity[_i]->x == this->level.entity[j]->x)
							this->level.entity.erase(this->level.entity.begin()+j);
					}
				}
				// Checking for collision ahead of the piece before moving it, then moving it if there isn't a wall
				if (this->level.mapLayerCollision[this->level.entity[_i]->y][this->level.entity[_i]->x] == 0)
					this->level.entity[_i]->y--;
				break;
			case 1:		// ================ Down ================
				// Capturing a piece if the moving piece moves on top of said piece
				for (int j = 0; j < this->level.entities; j++) {
					if (_i != j and this->level.entity[_i]->team != this->level.entity[j]->team) {
						if (this->level.entity[_i]->y + 1 == this->level.entity[j]->y and this->level.entity[_i]->x == this->level.entity[j]->x)
							this->level.entity.erase(this->level.entity.begin()+j);
					}
				}
				// Checking for collision ahead of the piece before moving it, then moving it if there isn't a wall
				if (this->level.mapLayerCollision[this->level.entity[_i]->y+2][this->level.entity[_i]->x] == 0)
					this->level.entity[_i]->y++;
				break;
			case 2:		// ================ Left ================
				// Capturing a piece if the moving piece moves on top of said piece
				for (int j = 0; j < this->level.entities; j++) {
					if (_i != j and this->level.entity[_i]->team != this->level.entity[j]->team) {
						if (this->level.entity[_i]->x - 1 == this->level.entity[j]->x and this->level.entity[_i]->y == this->level.entity[j]->y)
							this->level.entity.erase(this->level.entity.begin()+j);
					}
				}
				// Checking for collision ahead of the piece before moving it, then moving it if there isn't a wall
				if (this->level.mapLayerCollision[this->level.entity[_i]->y+1][this->level.entity[_i]->x-1] == 0)
					this->level.entity[_i]->x--;
				break;
		case 3:		// ================ Right ================
			// Capturing a piece if the moving piece moves on top of said piece
			for (int j = 0; j < this->level.entities; j++) {
				if (_i != j and this->level.entity[_i]->team != this->level.entity[j]->team) {
					if (this->level.entity[_i]->x + 1 == this->level.entity[j]->x and this->level.entity[_i]->y == this->level.entity[j]->y)
						this->level.entity.erase(this->level.entity.begin()+j);
				}
			}
			// Checking for collision ahead of the piece before moving it, then moving it if there isn't a wall
			if (this->level.mapLayerCollision[this->level.entity[_i]->y+1][this->level.entity[_i]->x+1] == 0)
				this->level.entity[_i]->x++;
			break;
			case 4:		// ================ Up-Right ================
				// Capturing a piece if the moving piece moves on top of said piece
				for (int j = 0; j < this->level.entities; j++) {
					if (_i != j and this->level.entity[_i]->team != this->level.entity[j]->team) {
						if (this->level.entity[_i]->x + 1 == this->level.entity[j]->x and this->level.entity[_i]->y - 1 == this->level.entity[j]->y)
							this->level.entity.erase(this->level.entity.begin()+j);
					}
				}
				// Checking for collision ahead of the piece before moving it, then moving it if there isn't a wall
				if (this->level.mapLayerCollision[this->level.entity[_i]->y][this->level.entity[_i]->x+1] == 0) {
					this->level.entity[_i]->x++;
					this->level.entity[_i]->y--;
				}
				break;
			case 5:		// ================ Up-Left ================
				// Capturing a piece if the moving piece moves on top of said piece
				for (int j = 0; j < this->level.entities; j++) {
					if (_i != j and this->level.entity[_i]->team != this->level.entity[j]->team) {
						if (this->level.entity[_i]->x - 1 == this->level.entity[j]->x and this->level.entity[_i]->y - 1 == this->level.entity[j]->y)
							this->level.entity.erase(this->level.entity.begin()+j);
					}
				}
				// Checking for collision ahead of the piece before moving it, then moving it if there isn't a wall
				if (this->level.mapLayerCollision[this->level.entity[_i]->y][this->level.entity[_i]->x-1] == 0) {
					this->level.entity[_i]->x--;
					this->level.entity[_i]->y--;
				}
				break;
			case 6:		// ================ Down-Right ================
				// Capturing a piece if the moving piece moves on top of said piece
				for (int j = 0; j < this->level.entities; j++) {
					if (_i != j and this->level.entity[_i]->team != this->level.entity[j]->team) {
						if (this->level.entity[_i]->x + 1 == this->level.entity[j]->x and this->level.entity[_i]->y + 1 == this->level.entity[j]->y)
							this->level.entity.erase(this->level.entity.begin()+j);
					}
				}
				// Checking for collision ahead of the piece before moving it, then moving it if there isn't a wall
				if (this->level.mapLayerCollision[this->level.entity[_i]->y+2][this->level.entity[_i]->x+1] == 0) {
					this->level.entity[_i]->x++;
					this->level.entity[_i]->y++;
				}
				break;
			case 7:		// ================ Down-Left ================
				// Capturing a piece if the moving piece moves on top of said piece
				for (int j = 0; j < this->level.entities; j++) {
					if (_i != j and this->level.entity[_i]->team != this->level.entity[j]->team) {
						if (this->level.entity[_i]->x - 1 == this->level.entity[j]->x and this->level.entity[_i]->y + 1 == this->level.entity[j]->y)
							this->level.entity.erase(this->level.entity.begin()+j);
					}
				}
				// Checking for collision ahead of the piece before moving it, then moving it if there isn't a wall
				if (this->level.mapLayerCollision[this->level.entity[_i]->y+2][this->level.entity[_i]->x-1] == 0) {
					this->level.entity[_i]->x--;
					this->level.entity[_i]->y++;
				}
				break;
		}
	}

	// The Queen piece's moves
	else if (this->level.entity[_i]->piece == "Queen") {
		int _length = 0, _maxLength = 0;
		switch (this->level.entity[_i]->dir) {
			case 0:		// ================ Up ================
				// Randomizing how far the piece will go
				while (this->level.mapLayerCollision[this->level.entity[_i]->y-_maxLength+1][this->level.entity[_i]->x] == 0) {
					_maxLength++;
				}
				_length = std::rand() % _maxLength;

				// Capturing a piece if the moving piece moves on top of said piece
				for (int j = 0; j < this->level.entities; j++) {
					if (_i != j and this->level.entity[_i]->team != this->level.entity[j]->team) {
						if (this->level.entity[_i]->y - _length == this->level.entity[j]->y and this->level.entity[_i]->x == this->level.entity[j]->x)
							this->level.entity.erase(this->level.entity.begin()+j);
					}
				}
				// Checking for collision ahead of the piece before moving it, then moving it if there isn't a wall
				this->level.entity[_i]->y -= _length;
				break;
			case 1:		// ================ Down ================
				// Randomizing how far the piece will go
				while (this->level.mapLayerCollision[this->level.entity[_i]->y+_maxLength+1][this->level.entity[_i]->x] == 0) {
					_maxLength++;
				}
				_length = std::rand() % _maxLength;

				// Capturing a piece if the moving piece moves on top of said piece
				for (int j = 0; j < this->level.entities; j++) {
					if (_i != j and this->level.entity[_i]->team != this->level.entity[j]->team) {
						if (this->level.entity[_i]->y + _length == this->level.entity[j]->y and this->level.entity[_i]->x == this->level.entity[j]->x)
							this->level.entity.erase(this->level.entity.begin()+j);
					}
				}
				// Checking for collision ahead of the piece before moving it, then moving it if there isn't a wall
				if (this->level.mapLayerCollision[this->level.entity[_i]->y+2][this->level.entity[_i]->x] == 0)
					this->level.entity[_i]->y += _length;
				break;
			case 2:		// ================ Left ================
				// Randomizing how far the piece will go
				while (this->level.mapLayerCollision[this->level.entity[_i]->y+1][this->level.entity[_i]->x-_maxLength] == 0) {
					_maxLength++;
				}
				_length = std::rand() % _maxLength;

				// Capturing a piece if the moving piece moves on top of said piece
				for (int j = 0; j < this->level.entities; j++) {
					if (_i != j and this->level.entity[_i]->team != this->level.entity[j]->team) {
						if (this->level.entity[_i]->x - _length == this->level.entity[j]->x and this->level.entity[_i]->y == this->level.entity[j]->y)
							this->level.entity.erase(this->level.entity.begin()+j);
					}
				}
				// Checking for collision ahead of the piece before moving it, then moving it if there isn't a wall
				if (this->level.mapLayerCollision[this->level.entity[_i]->y+1][this->level.entity[_i]->x-1] == 0)
					this->level.entity[_i]->x -= _length;
				break;
			case 3:		// ================ Right ================
				// Randomizing how far the piece will go
				while (this->level.mapLayerCollision[this->level.entity[_i]->y+1][this->level.entity[_i]->x+_maxLength] == 0) {
					_maxLength++;
				}
				_length = std::rand() % _maxLength;

				// Capturing a piece if the moving piece moves on top of said piece
				for (int j = 0; j < this->level.entities; j++) {
					if (_i != j and this->level.entity[_i]->team != this->level.entity[j]->team) {
						if (this->level.entity[_i]->x + _length == this->level.entity[j]->x and this->level.entity[_i]->y == this->level.entity[j]->y)
							this->level.entity.erase(this->level.entity.begin()+j);
					}
				}
				// Checking for collision ahead of the piece before moving it, then moving it if there isn't a wall
				if (this->level.mapLayerCollision[this->level.entity[_i]->y+1][this->level.entity[_i]->x+1] == 0)
					this->level.entity[_i]->x += _length;
				break;
			case 4:		// ================ Up-Right ================
				// Randomizing how far the piece will go
				while (this->level.mapLayerCollision[this->level.entity[_i]->y-_maxLength+1][this->level.entity[_i]->x+_maxLength] == 0) {
					_maxLength++;
				}
				_length = std::rand() % _maxLength;

				// Capturing a piece if the moving piece moves on top of said piece
				for (int j = 0; j < this->level.entities; j++) {
					if (_i != j and this->level.entity[_i]->team != this->level.entity[j]->team) {
						if (this->level.entity[_i]->x + _length == this->level.entity[j]->x and this->level.entity[_i]->y - _length == this->level.entity[j]->y)
							this->level.entity.erase(this->level.entity.begin()+j);
					}
				}
				// Checking for collision ahead of the piece before moving it, then moving it if there isn't a wall
				if (this->level.mapLayerCollision[this->level.entity[_i]->y][this->level.entity[_i]->x+1] == 0) {
					this->level.entity[_i]->x += _length;
					this->level.entity[_i]->y -= _length;
				}
				break;
			case 5:		// ================ Up-Left ================
				// Randomizing how far the piece will go
				while (this->level.mapLayerCollision[this->level.entity[_i]->y-_maxLength+1][this->level.entity[_i]->x-_maxLength] == 0) {
					_maxLength++;
				}
				_length = std::rand() % _maxLength;

				// Capturing a piece if the moving piece moves on top of said piece
				for (int j = 0; j < this->level.entities; j++) {
					if (_i != j and this->level.entity[_i]->team != this->level.entity[j]->team) {
						if (this->level.entity[_i]->x - _length == this->level.entity[j]->x and this->level.entity[_i]->y - _length == this->level.entity[j]->y)
							this->level.entity.erase(this->level.entity.begin()+j);
					}
				}
				// Checking for collision ahead of the piece before moving it, then moving it if there isn't a wall
				if (this->level.mapLayerCollision[this->level.entity[_i]->y][this->level.entity[_i]->x-1] == 0) {
					this->level.entity[_i]->x -= _length;
					this->level.entity[_i]->y -= _length;
				}
				break;
			case 6:		// ================ Down-Right ================
				// Randomizing how far the piece will go
				while (this->level.mapLayerCollision[this->level.entity[_i]->y+_maxLength+1][this->level.entity[_i]->x+_maxLength] == 0) {
					_maxLength++;
				}
				_length = std::rand() % _maxLength;

				// Capturing a piece if the moving piece moves on top of said piece
				for (int j = 0; j < this->level.entities; j++) {
					if (_i != j and this->level.entity[_i]->team != this->level.entity[j]->team) {
						if (this->level.entity[_i]->x + _length == this->level.entity[j]->x and this->level.entity[_i]->y + _length == this->level.entity[j]->y)
							this->level.entity.erase(this->level.entity.begin()+j);
					}
				}
				// Checking for collision ahead of the piece before moving it, then moving it if there isn't a wall
				if (this->level.mapLayerCollision[this->level.entity[_i]->y+2][this->level.entity[_i]->x+1] == 0) {
					this->level.entity[_i]->x += _length;
					this->level.entity[_i]->y += _length;
				}
				break;
			case 7:		// ================ Down-Left ================
				// Randomizing how far the piece will go
				while (this->level.mapLayerCollision[this->level.entity[_i]->y+_maxLength+1][this->level.entity[_i]->x-_maxLength] == 0) {
					_maxLength++;
				}
				_length = std::rand() % _maxLength;

				// Capturing a piece if the moving piece moves on top of said piece
				for (int j = 0; j < this->level.entities; j++) {
					if (_i != j and this->level.entity[_i]->team != this->level.entity[j]->team) {
						if (this->level.entity[_i]->x - _length == this->level.entity[j]->x and this->level.entity[_i]->y + _length == this->level.entity[j]->y)
							this->level.entity.erase(this->level.entity.begin()+j);
					}
				}
				// Checking for collision ahead of the piece before moving it, then moving it if there isn't a wall
				if (this->level.mapLayerCollision[this->level.entity[_i]->y+2][this->level.entity[_i]->x-1] == 0) {
					this->level.entity[_i]->x -= _length;
					this->level.entity[_i]->y += _length;
				}
				break;
		}
	}
	else if (this->level.entity[_i]->piece == "Bishop") {
		int _length = 0, _maxLength = 0;
		switch (this->level.entity[_i]->dir) {
			case 0:		// ================ Up-Right ================
				// Randomizing how far the piece will go
				while (this->level.mapLayerCollision[this->level.entity[_i]->y-_maxLength+1][this->level.entity[_i]->x+_maxLength] == 0) {
					_maxLength++;
				}
				_length = std::rand() % _maxLength;
				
				// Capturing a piece if the moving piece moves on top of said piece
				for (int j = 0; j < this->level.entities; j++) {
					if (_i != j and this->level.entity[_i]->team != this->level.entity[j]->team) {
						if (this->level.entity[_i]->x + _length == this->level.entity[j]->x and this->level.entity[_i]->y - _length == this->level.entity[j]->y)
							this->level.entity.erase(this->level.entity.begin()+j);
					}
				}
				// Checking for collision ahead of the piece before moving it, then moving it if there isn't a wall
				this->level.entity[_i]->x += _length;
				this->level.entity[_i]->y -= _length;
				break;
			case 1:		// ================ Up-Left ================
				// Randomizing how far the piece will go
				while (this->level.mapLayerCollision[this->level.entity[_i]->y-_maxLength+1][this->level.entity[_i]->x-_maxLength] == 0) {
					_maxLength++;
				}
				_length = std::rand() % _maxLength;

				// Capturing a piece if the moving piece moves on top of said piece
				for (int j = 0; j < this->level.entities; j++) {
					if (_i != j and this->level.entity[_i]->team != this->level.entity[j]->team) {
						if (this->level.entity[_i]->x - _length == this->level.entity[j]->x and this->level.entity[_i]->y - _length == this->level.entity[j]->y)
							this->level.entity.erase(this->level.entity.begin()+j);
					}
				}
				// Checking for collision ahead of the piece before moving it, then moving it if there isn't a wall
				this->level.entity[_i]->x -= _length;
				this->level.entity[_i]->y -= _length;
				break;
			case 2:		// ================ Down-Right ================
				// Randomizing how far the piece will go
				while (this->level.mapLayerCollision[this->level.entity[_i]->y+_maxLength+1][this->level.entity[_i]->x+_maxLength] == 0) {
					_maxLength++;
				}
				_length = std::rand() % _maxLength;

				// Capturing a piece if the moving piece moves on top of said piece
				for (int j = 0; j < this->level.entities; j++) {
					if (_i != j and this->level.entity[_i]->team != this->level.entity[j]->team) {
						if (this->level.entity[_i]->x + _length == this->level.entity[j]->x and this->level.entity[_i]->y + _length == this->level.entity[j]->y)
							this->level.entity.erase(this->level.entity.begin()+j);
					}
				}
				// Checking for collision ahead of the piece before moving it, then moving it if there isn't a wall
				this->level.entity[_i]->x += _length;
				this->level.entity[_i]->y += _length;
				break;
			case 3:		// ================ Down-Left ================
				// Randomizing how far the piece will go
				while (this->level.mapLayerCollision[this->level.entity[_i]->y+_maxLength+1][this->level.entity[_i]->x-_maxLength] == 0) {
					_maxLength++;
				}
				_length = std::rand() % _maxLength;

				// Capturing a piece if the moving piece moves on top of said piece
				for (int j = 0; j < this->level.entities; j++) {
					if (_i != j and this->level.entity[_i]->team != this->level.entity[j]->team) {
						if (this->level.entity[_i]->x - _length == this->level.entity[j]->x and this->level.entity[_i]->y + _length == this->level.entity[j]->y)
							this->level.entity.erase(this->level.entity.begin()+j);
						}
				}
				// Checking for collision ahead of the piece before moving it, then moving it if there isn't a wall
				this->level.entity[_i]->x -= _length;
				this->level.entity[_i]->y += _length;
				break;
		}
	}
	else if (this->level.entity[_i]->piece == "Knight") {
		switch (this->level.entity[_i]->dir) {
			case 0:		// ================ Up-Left ================
				// Capturing a piece if the moving piece moves on top of said piece
				for (int j = 0; j < this->level.entities; j++) {
					if (_i != j and this->level.entity[_i]->team != this->level.entity[j]->team) {
						if (this->level.entity[_i]->x - 1 == this->level.entity[j]->x and this->level.entity[_i]->y - 2 == this->level.entity[j]->y)
							this->level.entity.erase(this->level.entity.begin()+j);
					}
				}
				// Checking for collision ahead of the piece before moving it, then moving it if there isn't a wall
				if (this->level.mapLayerCollision[this->level.entity[_i]->y-1][this->level.entity[_i]->x-1] == 0) {
					this->level.entity[_i]->x--;
					this->level.entity[_i]->y -= 2;
				}
				break;
			case 1:		// ================ Up-Right ================
				// Capturing a piece if the moving piece moves on top of said piece
				for (int j = 0; j < this->level.entities; j++) {
					if (_i != j and this->level.entity[_i]->team != this->level.entity[j]->team) {
						if (this->level.entity[_i]->x + 1 == this->level.entity[j]->x and this->level.entity[_i]->y - 2 == this->level.entity[j]->y)
							this->level.entity.erase(this->level.entity.begin()+j);
					}
				}
				// Checking for collision ahead of the piece before moving it, then moving it if there isn't a wall
				if (this->level.mapLayerCollision[this->level.entity[_i]->y-1][this->level.entity[_i]->x+1] == 0) {
					this->level.entity[_i]->x++;
					this->level.entity[_i]->y -= 2;
				}
				break;
			case 2:		// ================ Right-Up ================
				// Capturing a piece if the moving piece moves on top of said piece
				for (int j = 0; j < this->level.entities; j++) {
					if (_i != j and this->level.entity[_i]->team != this->level.entity[j]->team) {
						if (this->level.entity[_i]->x + 2 == this->level.entity[j]->x and this->level.entity[_i]->y - 1 == this->level.entity[j]->y)
							this->level.entity.erase(this->level.entity.begin()+j);
					}
				}
				// Checking for collision ahead of the piece before moving it, then moving it if there isn't a wall
				if (this->level.mapLayerCollision[this->level.entity[_i]->y][this->level.entity[_i]->x+2] == 0) {
					this->level.entity[_i]->x += 2;
					this->level.entity[_i]->y--;
				}
				break;
			case 3:		// ================ Right-Down ================
				// Capturing a piece if the moving piece moves on top of said piece
				for (int j = 0; j < this->level.entities; j++) {
					if (_i != j and this->level.entity[_i]->team != this->level.entity[j]->team) {
						if (this->level.entity[_i]->x + 2 == this->level.entity[j]->x and this->level.entity[_i]->y + 1 == this->level.entity[j]->y)
							this->level.entity.erase(this->level.entity.begin()+j);
					}
				}
				// Checking for collision ahead of the piece before moving it, then moving it if there isn't a wall
				if (this->level.mapLayerCollision[this->level.entity[_i]->y+2][this->level.entity[_i]->x+2] == 0) {
					this->level.entity[_i]->x += 2;
					this->level.entity[_i]->y++;
				}
				break;
			case 4:		// ================ Down-Left ================
				// Capturing a piece if the moving piece moves on top of said piece
				for (int j = 0; j < this->level.entities; j++) {
					if (_i != j and this->level.entity[_i]->team != this->level.entity[j]->team) {
						if (this->level.entity[_i]->x - 1 == this->level.entity[j]->x and this->level.entity[_i]->y + 2 == this->level.entity[j]->y)
							this->level.entity.erase(this->level.entity.begin()+j);
					}
				}
				// Checking for collision ahead of the piece before moving it, then moving it if there isn't a wall
				if (this->level.mapLayerCollision[this->level.entity[_i]->y+3][this->level.entity[_i]->x-1] == 0) {
					this->level.entity[_i]->x--;
					this->level.entity[_i]->y += 2;
				}
				break;
			case 5:		// ================ Down-Right ================
				// Capturing a piece if the moving piece moves on top of said piece
				for (int j = 0; j < this->level.entities; j++) {
					if (_i != j and this->level.entity[_i]->team != this->level.entity[j]->team) {
						if (this->level.entity[_i]->x + 1 == this->level.entity[j]->x and this->level.entity[_i]->y + 2 == this->level.entity[j]->y)
							this->level.entity.erase(this->level.entity.begin()+j);
					}
				}
				// Checking for collision ahead of the piece before moving it, then moving it if there isn't a wall
				if (this->level.mapLayerCollision[this->level.entity[_i]->y+3][this->level.entity[_i]->x+1] == 0) {
					this->level.entity[_i]->x++;
					this->level.entity[_i]->y += 2;
				}
				break;
			case 6:		// ================ Left-Up ================
				// Capturing a piece if the moving piece moves on top of said piece
				for (int j = 0; j < this->level.entities; j++) {
					if (_i != j and this->level.entity[_i]->team != this->level.entity[j]->team) {
						if (this->level.entity[_i]->x - 2 == this->level.entity[j]->x and this->level.entity[_i]->y - 1 == this->level.entity[j]->y)
							this->level.entity.erase(this->level.entity.begin()+j);
					}
				}
				// Checking for collision ahead of the piece before moving it, then moving it if there isn't a wall
				if (this->level.mapLayerCollision[this->level.entity[_i]->y][this->level.entity[_i]->x-2] == 0) {
					this->level.entity[_i]->x -= 2;
					this->level.entity[_i]->y--;
				}
				break;
			case 7:		// ================ Left-Down ================
				// Capturing a piece if the moving piece moves on top of said piece
				for (int j = 0; j < this->level.entities; j++) {
					if (_i != j and this->level.entity[_i]->team != this->level.entity[j]->team) {
						if (this->level.entity[_i]->x - 2 == this->level.entity[j]->x and this->level.entity[_i]->y + 1 == this->level.entity[j]->y)
							this->level.entity.erase(this->level.entity.begin()+j);
					}
				}
				// Checking for collision ahead of the piece before moving it, then moving it if there isn't a wall
				if (this->level.mapLayerCollision[this->level.entity[_i]->y+2][this->level.entity[_i]->x-2] == 0) {
					this->level.entity[_i]->x -= 2;
					this->level.entity[_i]->y++;
				}
				break;
		}
	}
	else if (this->level.entity[_i]->piece == "Rook") {
		int _length = 0, _maxLength = 0;
		switch (this->level.entity[_i]->dir) {
			case 0:		// ================ Up ================
				// Randomizing how far the piece will go
				while (this->level.mapLayerCollision[this->level.entity[_i]->y-_maxLength+1][this->level.entity[_i]->x] == 0) {
					_maxLength++;
				}
				_length = std::rand() % _maxLength;

				// Capturing a piece if the moving piece moves on top of said piece
				for (int j = 0; j < this->level.entities; j++) {
					if (_i != j and this->level.entity[_i]->team != this->level.entity[j]->team) {
						if (this->level.entity[_i]->y - _length == this->level.entity[j]->y and this->level.entity[_i]->x == this->level.entity[j]->x)
							this->level.entity.erase(this->level.entity.begin()+j);
					}
				}

				// Checking for collision ahead of the piece before moving it, then moving it if there isn't a wall
				this->level.entity[_i]->y -= _length;
				break;
			case 1:		// ================ Down ================
				// Randomizing how far the piece will go
				while (this->level.mapLayerCollision[this->level.entity[_i]->y+_maxLength+1][this->level.entity[_i]->x] == 0) {
					_maxLength++;
				}
				_length = std::rand() % _maxLength;

				// Capturing a piece if the moving piece moves on top of said piece
				for (int j = 0; j < this->level.entities; j++) {
					if (_i != j and this->level.entity[_i]->team != this->level.entity[j]->team) {
						if (this->level.entity[_i]->y + _length == this->level.entity[j]->y and this->level.entity[_i]->x == this->level.entity[j]->x)
							this->level.entity.erase(this->level.entity.begin()+j);
					}
				}

				// Checking for collision ahead of the piece before moving it, then moving it if there isn't a wall
				if (this->level.mapLayerCollision[this->level.entity[_i]->y+2][this->level.entity[_i]->x] == 0)
					this->level.entity[_i]->y += _length;
				break;
			case 2:		// ================ Left ================
				// Randomizing how far the piece will go
				while (this->level.mapLayerCollision[this->level.entity[_i]->y+1][this->level.entity[_i]->x-_maxLength] == 0) {
					_maxLength++;
				}
				_length = std::rand() % _maxLength;

				// Capturing a piece if the moving piece moves on top of said piece
				for (int j = 0; j < this->level.entities; j++) {
					if (_i != j and this->level.entity[_i]->team != this->level.entity[j]->team) {
						if (this->level.entity[_i]->x - _length == this->level.entity[j]->x and this->level.entity[_i]->y == this->level.entity[j]->y)
							this->level.entity.erase(this->level.entity.begin()+j);
					}
				}

				// Checking for collision ahead of the piece before moving it, then moving it if there isn't a wall
				if (this->level.mapLayerCollision[this->level.entity[_i]->y+1][this->level.entity[_i]->x-1] == 0)
					this->level.entity[_i]->x -= _length;
				break;
			case 3:		// ================ Right ================
				// Randomizing how far the piece will go
				while (this->level.mapLayerCollision[this->level.entity[_i]->y+1][this->level.entity[_i]->x+_maxLength] == 0) {
					_maxLength++;
				}
				_length = std::rand() % _maxLength;

				// Capturing a piece if the moving piece moves on top of said piece
				for (int j = 0; j < this->level.entities; j++) {
					if (_i != j and this->level.entity[_i]->team != this->level.entity[j]->team) {
						if (this->level.entity[_i]->x + _length == this->level.entity[j]->x and this->level.entity[_i]->y == this->level.entity[j]->y)
							this->level.entity.erase(this->level.entity.begin()+j);
					}
				}

				// Checking for collision ahead of the piece before moving it, then moving it if there isn't a wall
				if (this->level.mapLayerCollision[this->level.entity[_i]->y+1][this->level.entity[_i]->x+1] == 0)
					this->level.entity[_i]->x += _length;
				break;
		}
	}
	else if (this->level.entity[_i]->piece == "Pawn") {
		// Checking if this Pawn can capture a piece, and if so do it
		for (int j = 0; j < this->level.entities; j++) {
			if (this->level.entity[j]->x == this->level.entity[_i]->x - 1 and this->level.entity[j]->y == this->level.entity[_i]->y - 1) {
				if (this->level.entity[j]->team != "Enemy") {
					this->level.entity[_i]->dir = 1;
					this->level.entity.erase(this->level.entity.begin()+j);
				}
			}
			else if (this->level.entity[j]->x == this->level.entity[_i]->x + 1 and this->level.entity[j]->y == this->level.entity[_i]->y - 1) {
				if (this->level.entity[j]->team != "Enemy") {
					this->level.entity[_i]->dir = 2;
					this->level.entity.erase(this->level.entity.begin()+j);
				}
			}
		}
		bool _collide = false;
		switch (this->level.entity[_i]->dir) {
			case 0:		// ================ Up ================
				// Check for a piece ahead of this piece and if there's a piece there don't move
				for (int j = 0; j < this->level.entities; j++) {
					if (this->level.entity[_i]->y - 1 == this->level.entity[j]->y and this->level.entity[_i]->x == this->level.entity[j]->x)
						_collide = true;
				}
				// Checking for collision ahead of the piece before moving it, then moving it if there isn't a wall
				if (this->level.mapLayerCollision[this->level.entity[_i]->y][this->level.entity[_i]->x] == 0 and !_collide)
					this->level.entity[_i]->y--;
				break;
			case 1:		// ================ Up Left ================
				// Moving up-left
				this->level.entity[_i]->y--;
				this->level.entity[_i]->x--;
				break;
			case 2:		// ================ Up Right ================
				// Moving up-right
				this->level.entity[_i]->y--;
				this->level.entity[_i]->x++;
				break;
		}
	}
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
			case sf::Event::Closed:
				this->window.close();
				break;
			case sf::Event::KeyPressed:
				if (this->ev.key.code == sf::Keyboard::Escape)
					this->window.close();
				if (this->ev.key.code == sf::Keyboard::F11) {
					if (this->fullscreen) {
						this->level.entity[this->level.playerID]->fullscreen = 0;
						this->initWindow(false);
					}
					else {
						this->level.entity[this->level.playerID]->fullscreen = 1;
						this->initWindow(true);
					}
				}
				break;
		}
	}
}

// Adds a text object to the text layer for displaying later (ONLY CALL ONCE)
void Game::print(std::string _string, int _x, int _y, int _size, sf::Color _color) {
	sf::Text _text;
	_text.setFont(this->font);
	_text.setString(_string);
	_text.setPosition(_x, _y);
	_text.setCharacterSize(_size);
	_text.setFillColor(_color);
	this->textLayer.push_back(_text);
}