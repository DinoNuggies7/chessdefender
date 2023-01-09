#include "level.h"

// ==================================
//  		Public Functions
// ==================================

void Level::init(int _levelID, int _door0, int _door1, int _door2, int _door3) {
	// Spawns enemies depending on how deep you are in the dungeon
	if (_levelID == 0) {
		this->entity.push_back(new Player());
	}
	else {
		if (this->room[0][_levelID] == 0) {
			for (int i = 0; i < _levelID; i++) {
				this->entity.push_back(new Enemy());
				printf("Enemy created\n");
			}
		}
	}

	// Initializes Collision
	this->initCollision();

	// Initialize Doors
	this->initDoors(_door0, _door1, _door2, _door3);

	// Initialize Enitities
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

void Level::initDoors(int _door0, int _door1, int _door2, int _door3) {
	// Removes previous doors and clear collision layer of door locations BACKWORDS
	for (int i = this->entity.size()-1; i > 0; i--) {
		if (this->entity[i]->team == "Object") {
			this->entity.erase(this->entity.begin()+i);
		}
	}
	this->mapLayerCollision[0+1][14] = 0;
	this->mapLayerCollision[0+1][15] = 0;
	this->mapLayerCollision[7+1][29] = 0;
	this->mapLayerCollision[8+1][29] = 0;
	this->mapLayerCollision[15+1][14] = 0;
	this->mapLayerCollision[15+1][15] = 0;
	this->mapLayerCollision[8+1][0] = 0;
	this->mapLayerCollision[7+1][0] = 0;

	// Spawns doors and adds the two segments of the doors to the collision layer
	if (_door0 == -1) {
		this->entity.push_back(new Door(0));
		this->mapLayerCollision[0+1][14] = 1;
		this->mapLayerCollision[0+1][15] = 1;
		printf("one\n");
	}
	if (_door1 == -1) {
		this->entity.push_back(new Door(1));
		this->mapLayerCollision[7+1][29] = 1;
		this->mapLayerCollision[8+1][29] = 1;
		printf("two\n");
	}
	if (_door2 == -1) {
		this->entity.push_back(new Door(2));
		this->mapLayerCollision[15+1][14] = 1;
		this->mapLayerCollision[15+1][15] = 1;
		printf("three\n");
	}
	if (_door3 == -1) {
		this->entity.push_back(new Door(3));
		this->mapLayerCollision[8+1][0] = 1;
		this->mapLayerCollision[7+1][0] = 1;
		printf("four\n");
	}

	// Update `Level::playerID` with the index of the player entity
	this->playerID = -1;
	for (int i = 0; i < this->entities; i++) {
		if (this->entity[i]->team == "Player") {
			this->playerID = i;
		}
	}
}

void Level::randomizeRoomGen(int& _warp0, int& _warp1, int& _warp2) {
	int _rand = std::rand() & 6;
	switch (_rand) {
		case 0:
			_warp0 = this->currentRoom + 1;
			_warp1 = this->currentRoom + 2;
			_warp2 = this->currentRoom + 3;
			break;
		case 1:
			_warp0 = this->currentRoom + 1;
			_warp1 = this->currentRoom + 3;
			_warp2 = this->currentRoom + 2;
			break;
		case 2:
			_warp0 = this->currentRoom + 2;
			_warp1 = this->currentRoom + 1;
			_warp2 = this->currentRoom + 3;
			break;
		case 3:
			_warp0 = this->currentRoom + 2;
			_warp1 = this->currentRoom + 3;
			_warp2 = this->currentRoom + 1;
			break;
		case 4:
			_warp0 = this->currentRoom + 3;
			_warp1 = this->currentRoom + 1;
			_warp2 = this->currentRoom + 2;
			break;
		case 5:
			_warp0 = this->currentRoom + 3;
			_warp1 = this->currentRoom + 2;
			_warp2 = this->currentRoom + 1;
			break;
	}
}

void Level::generateRoom(int _warp0, int _warp1, int _warp2, int _warp3, int _clear) {
	this->room[0].push_back(_clear);
	this->room[1].push_back(_warp0);
	this->room[2].push_back(_warp1);
	this->room[3].push_back(_warp2);
	this->room[4].push_back(_warp3);
}

// ===================================
//  		Private Functions
// ===================================

void Level::initCollision() {
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
			while (gid_list[i-j] != ',' and i - j > 0)
				j++;
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
			while (gid_list[i-j] != ',')
				j++;
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
}

void Level::initEntities() {
	// Updates the amount of entities in the room
	this->entities = this->entity.size();

	// Updates Level::playerID
	for (int i = 0; i < this->entities; i++) {
		if (this->entity[i]->team == "Player")
			this->playerID = i;
	}

	// Initializing every entity
	for (int i = 0; i < this->entities; i++) {
		if (this->entity[i]->team != "Object") {
			if (this->entity[i]->team == "Player") {
				for (static bool _first0 = true; _first0; _first0 = false)
					this->entity[i]->init("King"); // The piece that the Player spawns with
			}
			else if (this->entity[i]->team == "Enemy") {
				int _rand = 4;//std::rand() % 6;
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
		else {
			this->entity[i]->init("Door");
		}
	}

	// Giving every entity a unique initiative
	std::vector<int> _illigalInit;
	for (int i = 0; i < this->entities; i++) {
		if (this->entity[i]->team != "Object") {
			int _rand = std::rand() % 19 + 1;
			bool _loop = true;
			while (_loop) {
			
				_loop = false;
				for (int j = 0; j < _illigalInit.size(); j++) {
					if (_rand == _illigalInit[j]) {
						_loop = true;
						_rand = std::rand() % 19 + 1;
					}
				}
			}
			this->entity[i]->initiative = _rand;
			_illigalInit.push_back(_rand);
		}
	}

	// Giving all enemies a random location
	for (int i = 1; i < this->entities; i++) {
		if (this->entity[i]->team != "Object") {
			while (this->mapLayerCollision[this->entity[i]->y+1][this->entity[i]->x] != 0 or this->entity[i]->x == this->entity[0]->x and this->entity[i]->y == this->entity[0]->y) {
				this->entity[i]->x = std::rand() % 27 + 1;
				this->entity[i]->y = std::rand() % 13 + 1;
			}
		}
	}

	// Displaying every entity's initiative
	printf("| ");
	for (int i = 0; i < this->entities; i++) {
		if (this->entity[i]->team != "Object")
			printf("%d | ", this->entity[i]->initiative);
	}
	printf("\n");
}