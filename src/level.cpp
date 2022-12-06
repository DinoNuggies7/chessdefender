#include "level.h"

// ==================================
//  		Public Functions
// ==================================

void Level::init(int _levelID) {
	// Spawning entities
	switch (_levelID) {
		case 0:
			this->entity.push_back(new Player);
			int _rand = std::rand() % 4 + 1;
			for (int i = 0; i < _rand; i++) {
				this->entity.push_back(new Enemy);
			}
			break;
	}
	this->entities = this->entity.size();

	// Setting up the map
	this->map.load("assets/level.tmx");
	this->mapLayerFloor.init(this->map, 0);
	this->mapLayerWalls1.init(this->map, 1);
	this->mapLayerWalls2.init(this->map, 2);
	this->mapLayerCeiling.init(this->map, 3);

	// Loading the collision tiles into the collision map layer
	tinyxml2::XMLDocument doc;
	doc.LoadFile("assets/level.tmx");

	tinyxml2::XMLElement* mapNode = doc.FirstChildElement("map");
	tinyxml2::XMLElement* pLayer = mapNode->FirstChildElement("layer");
	tinyxml2::XMLElement* pData = nullptr;

	while (pLayer != NULL) {
		const char* name = pLayer->Attribute("name");
		if (name == std::string("Collision"))
			pData = pLayer->FirstChildElement("data");
		pLayer = pLayer->NextSiblingElement("layer");
	}

	const char* gid_list = pData->GetText();
	size_t gid_listSize = strlen (gid_list); 
	std::stringstream value;
	value.str("");
	int row = 0;
	for(int i = 0; i < gid_listSize; i++) {
		if(gid_list[i] == ',') {
			value.str("");
			int j = 1;
			while (gid_list[i-j] != ',' and i - j > 0) {
				j++;
			}
			j--;
			value.str("");
			while (j > 0) {
				this->mapLayerCollision.emplace_back();
				value << gid_list[i-j];
				j--;
			}
			this->mapLayerCollision[row].push_back(std::stoi(value.str()));
			value.str("");
		}
		
		else if(gid_list[i] == '\n') {
			value.str("");
			int j = 1;
			while (gid_list[i-j] != ',') {
				j++;
			}
			j--;
			value.str("");
			while (j > 0) {
				value << gid_list[i-j];
				j--;
			}
			value.str("");
			row++;
		}
	}

	// Initializing enitities
	this->initEntities();
}

void Level::render(sf::RenderWindow& _window, int _layer) {
	if (!_layer) {
		_window.draw(this->mapLayerFloor);
		_window.draw(this->mapLayerWalls1);
		_window.draw(this->mapLayerWalls2);
	}
	else {
		_window.draw(this->mapLayerCeiling);
	}
}

// ===================================
//  		Private Functions
// ===================================

void Level::initEntities() {
	// Initializing every entity
	for (int i = 0; i < this->entities; i++) {
		if (i == 0)
			this->entity[i]->init("King"); // The piece that the Player spawns with
		else {
			int _rand = 5;//std::rand() % 6;
			if (_rand == 0)
				this->entity[i]->init("King");
			else if (_rand == 1)
				this->entity[i]->init("Queen");
			else if (_rand == 2)
				this->entity[i]->init("Bishop");
			else if (_rand == 3)
				this->entity[i]->init("Knight");
			else if (_rand == 4)
				this->entity[i]->init("Rook");
			else if (_rand == 5)
				this->entity[i]->init("Pawn");
		}
	}

	// Giving every entity a unique initiative
	std::vector<int> _illigalInit;
	for (int i = 0; i < this->entities; i++) {
	
		int _rand = std::rand() % 15 + 1;
		bool _loop = true;
		while (_loop) {
		
			_loop = false;
			for (int j = 0; j < _illigalInit.size(); j++) {
				if (_rand == _illigalInit[j]) {
					_loop = true;
					_rand = std::rand() % 15 + 1;
				}
			}
		}
	
		this->entity[i]->initiative = _rand;
		_illigalInit.push_back(_rand);
	}

	// Giving all entities a random location
	for (int i = 1; i < this->entities; i++) {
		while (this->mapLayerCollision[this->entity[i]->y+1][this->entity[i]->x] != 0 or this->entity[i]->x == this->entity[0]->x and this->entity[i]->y == this->entity[0]->y) {
			this->entity[i]->x = std::rand() % 30;
			this->entity[i]->y = std::rand() % 16;
		}
	}

	// Displaying every entity's initiative
	printf("| ");
	for (int i = 0; i < this->entities; i++)
		printf("%d | ", this->entity[i]->initiative);
	printf("\n");
}
