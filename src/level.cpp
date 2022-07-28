#include "level.h"

// ==================================
//  		Public Functions
// ==================================

void Level::init() {
	// Spawning entities
	this->entity.push_back(new Player);
	this->entity.push_back(new Enemy);
	this->entity.push_back(new Enemy);
	this->entity.push_back(new Enemy);
	this->entities = this->entity.size();

	// Initializing enitities
	this->initEntities();

	// Setting up the map
	this->map.load("assets/test_map.tmx");
	this->mapLayerFloor.init(this->map, 0);
	this->mapLayerWalls1.init(this->map, 1);
	this->mapLayerWalls2.init(this->map, 2);
	this->mapLayerCeiling.init(this->map, 3);

	// Loading the collision tiles into the collision map layer
	tinyxml2::XMLDocument doc;
	doc.LoadFile("assets/test_map.tmx");

	tinyxml2::XMLElement* mapNode = doc.FirstChildElement("map");
	tinyxml2::XMLElement* pLayer = mapNode->FirstChildElement("layer");
	tinyxml2::XMLElement* pData = nullptr;

	while (pLayer != NULL) {
		const char* name = pLayer->Attribute("name");
		if (name == std::string("Collision"))
			pData = pLayer->FirstChildElement("data");
		pLayer = pLayer->NextSiblingElement("layer");
	}

	std::string gid_list = pData->GetText();
	std::stringstream value;
	value.str("");
	int row = 0;
	for(int i = 0; i < gid_list.length(); i++) {
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
}

void Level::render(sf::RenderWindow* _window, int _layer) {
	if (!_layer) {
		_window->draw(this->mapLayerFloor);
		_window->draw(this->mapLayerWalls1);
		_window->draw(this->mapLayerWalls2);
	}
	else {
		_window->draw(this->mapLayerCeiling);
	}
}

// ===================================
//  		Private Functions
// ===================================

void Level::initEntities() {
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
