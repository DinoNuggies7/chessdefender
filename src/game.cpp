#include "game.h"

// ==================
//  Public Functions
// ==================

Game::Game() {
	// Randomizing seed for virtually truely random numbers
	std::srand(std::time(NULL));

	// Initializing Game class variables
	this->delta = 0;
	this->dt = 0;
	this->gameOver = false;
	this->step = 0;
	this->entityCounter = 0;

	// Spawning entities (not permanent, should move to level class)
	this->entity.push_back(new Player);
	this->entity.push_back(new Enemy);
	this->entity.push_back(new Enemy);
	this->entity.push_back(new Enemy);

	this->entities = this->entity.size();

	// Setting up the map (also not permanent, should move to level class)
	this->map.load("assets/test_map.tmx");
	this->mapLayerFloor.init(this->map, 0);
	this->mapLayerWall.init(this->map, 1);
	this->mapLayerCeiling.init(this->map, 2);

	// Loading the collision tiles into the collision map layer
	tinyxml2::XMLDocument doc;
	doc.LoadFile("assets/test_map.tmx");

	tinyxml2::XMLElement* mapNode = doc.FirstChildElement("map");
	tinyxml2::XMLElement* pLayer = mapNode->FirstChildElement("layer");
	tinyxml2::XMLElement* pData = nullptr;

	while (pLayer != NULL) {
		const char* name = pLayer->Attribute("name");
		if (name == std::string("Walls"))
			pData = pLayer->FirstChildElement("data");
		pLayer = pLayer->NextSiblingElement("layer");
	}

	std::cout << pData->GetText() << std::endl;
	std::string gid_list = pData->GetText();
	this->value.str("");
	int row = 0;
	for(int i = 0; i < gid_list.length(); i++) {
		if(gid_list[i] == ',') {
			this->value.str("");
			int j = 1;
			while (gid_list[i-j] != ',' and i - j > 0) {
				std::cout << gid_list[i-j] << " | ";
				j++;
				std::cout << gid_list[i-j] << std::endl;
			}
			j--;
			this->value.str("");
			std::cout << "j: " << j << std::endl;
			while (j > 0) {
				std::cout << "Inserting: " << gid_list[i-j] << std::endl;
				this->value << gid_list[i-j];
				std::cout << "Value AFTER: " << this->value.str() << std::endl;
				j--;
			}
			std::cout << "Putting into vector: " << this->value.str() << std::endl;
			//this->mapLayerCollision[row].push_back(this->value.str());
			this->value.str("");
		}
		
		else if(gid_list[i] == '\n') {
			this->value.str("");
			int j = 1;
			while (gid_list[i-j] != ',') {
				j++;
			}
			j--;
			this->value.str("");
			while (j > 0) {
				this->value << gid_list[i-j];
				j--;
			}
			std::cout << "Putting into vector: " << this->value.str() << std::endl;
			//this->mapLayerCollision[row].push_back(this->value.str());
			this->value.str("");
			row++;
		}
	}

	// More initialization
	this->initWindow();
	this->initEntities();
}

Game::~Game() {
	// Freeing up memory because pointers
	delete this->window;
	for (int i = 0; i < this->entities; i++) {
		delete this->entity[i];
		printf("Deleted Entity, ID: %d\n", i);
	}
}

bool Game::running() {
	// Just for neatness
	return this->window->isOpen();
}

void Game::update() {
	this->pollEvents();
	this->updateDelta();

	this->entities = this->entity.size();

	if (this->step > 16)
		this->step = 0;

	// Incrementing `step` if no entities have an initiative matching it
	if (this->entityCounter == this->entities)
		this->step++;

	// Making sure the camera always follows the player regardless of it's vector position
	this->playerID = -1;
	for (int i = 0; i < this->entities; i++) {
		if (this->entity[i]->team == "Player")
			this->playerID = i;
	}
	if (this->playerID == -1)
		this->gameOver = true;

	this->entityCounter = 0;
	for (int i = 0; i < this->entities; i++) {
		// Call the update function for every entity
		this->entity[i]->update(this->dt);

		// Moving, capturing pieces, and collision detection
		switch (this->entity[i]->dir) {
			case 0:
				for (int j = 0; j < this->entities; j++) {
					if (i != j and this->entity[i]->team != this->entity[j]->team) {
						if (this->entity[i]->y - 1 == this->entity[j]->y and this->entity[i]->x == this->entity[j]->x)
							this->entity.erase(this->entity.begin()+j);
					}
				}
				if (this->_mapLayerCollision[this->entity[i]->y-1][this->entity[i]->x] == 0)
					this->entity[i]->y--;
				this->entity[i]->dir = -1;
				break;
			case 1:
				for (int j = 0; j < this->entities; j++) {
					if (i != j and this->entity[i]->team != this->entity[j]->team) {
						if (this->entity[i]->y + 1 == this->entity[j]->y and this->entity[i]->x == this->entity[j]->x)
							this->entity.erase(this->entity.begin()+j);
					}
				}
				if (this->_mapLayerCollision[this->entity[i]->y+1][this->entity[i]->x] == 0)
					this->entity[i]->y++;
				this->entity[i]->dir = -1;
				break;
			case 2:
				for (int j = 0; j < this->entities; j++) {
					if (i != j and this->entity[i]->team != this->entity[j]->team) {
						if (this->entity[i]->x - 1 == this->entity[j]->x and this->entity[i]->y == this->entity[j]->y)
							this->entity.erase(this->entity.begin()+j);
					}
				}
				if (this->_mapLayerCollision[this->entity[i]->y][this->entity[i]->x-1] == 0)
					this->entity[i]->x--;
				this->entity[i]->dir = -1;
				break;
			case 3:
				for (int j = 0; j < this->entities; j++) {
					if (i != j and this->entity[i]->team != this->entity[j]->team) {
						if (this->entity[i]->x + 1 == this->entity[j]->x and this->entity[i]->y == this->entity[j]->y)
							this->entity.erase(this->entity.begin()+j);
					}
				}
				if (this->_mapLayerCollision[this->entity[i]->y][this->entity[i]->x+1] == 0)
					this->entity[i]->x++;
				this->entity[i]->dir = -1;
				break;
		}

		// Making sure one entity moves at a time
		if (this->step == this->entity[i]->initiative)
			this->entity[i]->turn = true;
		else {
			this->entity[i]->turn = false;
			this->entityCounter++;
		}
		if (this->entity[i]->doStep) {
			this->step++;
			this->entity[i]->doStep = false;
			this->entity[i]->turn = false;
		}
	}
}

void Game::render() {
	this->window->clear(sf::Color::Black);

	// Rendering bottom map layers
	this->window->draw(this->mapLayerFloor);
	this->window->draw(this->mapLayerWall);

	//Moving the entities around in the vector so that the ones with highest Y this->value get displayed last
	std::sort(this->entity.begin(), this->entity.end(), [] (const auto& lhs, const auto& rhs) {
		return lhs->y < rhs->y;
	});

	// Rendering every entity
	for (int i = 0; i < this->entities; i++) {
		this->entity[i]->render(this->window);
	}

	// Rendering top map layers (for ceilings, etc.)
	this->window->draw(this->mapLayerCeiling);

	// Camera that follows the player
	if (!this->gameOver) {
		this->view.setCenter(this->entity[this->playerID]->x * 16, this->entity[this->playerID]->y * 16);
		this->window->setView(this->view);
	}
	else {
		for (static bool _first = true; _first; _first = false)
			this->print("You Died", Global::WIN_WIDTH / 2 - 96, Global::WIN_HEIGHT / 2 - 16, 36, sf::Color(200, 20, 30, 255));
	}

	// Rendering all the stored text
	if (this->text.size() > 0) {
		this->window->setView(this->window->getDefaultView());
		for (int i = 0; i < this->text.size(); i++) {
			this->window->draw(this->text[i]);
		}
		this->window->setView(this->view);
	}

	this->window->display();
}

// ===================
//  Private Functions
// ===================

void Game::initEntities() {
	// Initializing every entity and randomizing their initiative
	for (int i = 0; i < this->entities; i++) {
		this->entity[i]->init();
		this->entity[i]->initiative = std::rand() % 15 + 1;
	}

	// Making sure that the player has a unique initiative (not permanent)
	if (this->entities > 3) {
		while (this->entity[0]->initiative == this->entity[1]->initiative or this->entity[0]->initiative == this->entity[2]->initiative or this->entity[0]->initiative == this->entity[3]->initiative)
			this->entity[0]->initiative = std::rand() % 15 + 1;

		// Displaying every entity's initiative
		printf("%d | %d | %d | %d \n", this->entity[0]->initiative, this->entity[1]->initiative, this->entity[2]->initiative, this->entity[3]->initiative);
	}
}

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


void Game::updateDelta() {
	this->delta = this->clock.restart().asSeconds();
	this->dt = delta * Global::FRAMERATE;
}

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

void Game::print(std::string _string, int _x, int _y, int _size, sf::Color _color) {
	sf::Text _text;
	_text.setFont(this->font);
	_text.setString(_string);
	_text.setPosition(_x, _y);
	_text.setCharacterSize(_size);
	_text.setFillColor(_color);
	this->text.push_back(_text);
}