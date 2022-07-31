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
	this->initWindow();
	this->level.init();
}

Game::~Game() {
	// Freeing up memory because pointers
	delete this->window;
	for (int i = 0; i < this->level.entities; i++) {
		delete this->level.entity[i];
		printf("Deleted Entity, ID: %d\n", i);
	}
}

bool Game::running() {
	return this->window->isOpen(); // Just for neatness
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
		this->level.entity[i]->update(this->dt);

		// Moving, capturing pieces, and collision detection
		switch (this->level.entity[i]->dir) {
			case 0:		// ================ Up ================
				for (int j = 0; j < this->level.entities; j++) {
					if (i != j and this->level.entity[i]->team != this->level.entity[j]->team) {
						if (this->level.entity[i]->y - 1 == this->level.entity[j]->y and this->level.entity[i]->x == this->level.entity[j]->x)
							this->level.entity.erase(this->level.entity.begin()+j);
					}
				}
				if (this->level.mapLayerCollision[this->level.entity[i]->y][this->level.entity[i]->x] == 0)
					this->level.entity[i]->y--;
				this->level.entity[i]->dir = -1;
				break;
			case 1:		// ================ Down ================
				for (int j = 0; j < this->level.entities; j++) {
					if (i != j and this->level.entity[i]->team != this->level.entity[j]->team) {
						if (this->level.entity[i]->y + 1 == this->level.entity[j]->y and this->level.entity[i]->x == this->level.entity[j]->x)
							this->level.entity.erase(this->level.entity.begin()+j);
					}
				}
				if (this->level.mapLayerCollision[this->level.entity[i]->y+2][this->level.entity[i]->x] == 0)
					this->level.entity[i]->y++;
				this->level.entity[i]->dir = -1;
				break;
			case 2:		// ================ Left ================
				for (int j = 0; j < this->level.entities; j++) {
					if (i != j and this->level.entity[i]->team != this->level.entity[j]->team) {
						if (this->level.entity[i]->x - 1 == this->level.entity[j]->x and this->level.entity[i]->y == this->level.entity[j]->y)
							this->level.entity.erase(this->level.entity.begin()+j);
					}
				}
				if (this->level.mapLayerCollision[this->level.entity[i]->y+1][this->level.entity[i]->x-1] == 0)
					this->level.entity[i]->x--;
				this->level.entity[i]->dir = -1;
				break;
			case 3:		// ================ Right ================
				for (int j = 0; j < this->level.entities; j++) {
					if (i != j and this->level.entity[i]->team != this->level.entity[j]->team) {
						if (this->level.entity[i]->x + 1 == this->level.entity[j]->x and this->level.entity[i]->y == this->level.entity[j]->y)
							this->level.entity.erase(this->level.entity.begin()+j);
					}
				}
				if (this->level.mapLayerCollision[this->level.entity[i]->y+1][this->level.entity[i]->x+1] == 0)
					this->level.entity[i]->x++;
				this->level.entity[i]->dir = -1;
				break;
		}

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
	this->window->clear(sf::Color::Black);

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

	// Camera that follows the player
	if (!this->gameOver) {
		int _x = this->level.entity[this->level.playerID]->x * 16;
		int _y = this->level.entity[this->level.playerID]->y * 16;
		this->view.setCenter(_x, _y);
		this->window->setView(this->view);
	}
	else {
		for (static bool _first = true; _first; _first = false)
			this->print("You Died", Global::WIN_WIDTH / 2 - 96, Global::WIN_HEIGHT / 2 - 16, 36, sf::Color(200, 20, 30, 255));
	}

	// Rendering all the stored text
	if (this->textLayer.size() > 0) {
		this->window->setView(this->window->getDefaultView());
		for (int i = 0; i < this->textLayer.size(); i++) {
			this->window->draw(this->textLayer[i]);
		}
		this->window->setView(this->view);
	}

	this->window->display();
}

// ===================================
//  		Private Functions
// ===================================

// Creates a new sf::RenderWindow, creates a view 3 times smaller and applys it along with other window settings, then finally initializes the global font
void Game::initWindow() {
	this->view.setSize(Global::WIDTH, Global::HEIGHT);
	this->view.setCenter(Global::WIDTH / 2, Global::HEIGHT / 2);
	this->videoMode = sf::VideoMode(Global::WIN_WIDTH, Global::WIN_HEIGHT);
	this->window = new sf::RenderWindow(this->videoMode, "Chess Defender", sf::Style::Titlebar | sf::Style::Close);
	this->window->setView(this->view);
	this->window->setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width / 2 - Global::WIN_WIDTH / 2, sf::VideoMode::getDesktopMode().height / 2 - Global::WIN_HEIGHT / 2));
	this->window->setKeyRepeatEnabled(false);
	this->font.loadFromFile("assets/Ravenna.ttf");
}

// Updated `dt` every frame, just in case I need frame independant animations
void Game::updateDelta() {
	this->delta = this->clock.restart().asSeconds();
	this->dt = delta * Global::FRAMERATE;
}

// The regular SFML window events
void Game::pollEvents() {
	while (this->window->pollEvent(ev)) {
		switch (this->ev.type) {
			case sf::Event::Closed:
				this->window->close();
				break;
			case sf::Event::KeyPressed:
				if (ev.key.code == sf::Keyboard::Escape)
					this->window->close();
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