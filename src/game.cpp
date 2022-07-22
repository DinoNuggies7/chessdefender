#include "game.h"

// Public Functions
Game::Game() {
	this->delta = 0;
	this->dt = 0;
	this->step = 0;
	this->entities = 2;
	this->entity[0] = new Player();
	this->entity[1] = new Enemy();
	this->initWindow();
	this->initEntities();
}

Game::~Game() {
	delete this->window;
}

bool Game::running() {
	return this->window->isOpen();
}

void Game::update() {
	this->pollEvents();
	this->updateDelta();
	
	for (int i = 0; i < this->entities - 1; i++) {
		this->entity[i]->update(this->dt);

		if (this->step == this->entity[i]->initiative)
			this->entity[i]->turn = true;
		else
			this->step++;

		if (this->entity[i]->doStep) {
			this->step++;
			this->entity[i]->doStep = false;
			this->entity[i]->turn = false;
		}
	}
}

void Game::render() {
	this->window->clear(sf::Color::Black);

	for (int i = 0; i < this->entities; i++) {
		this->entity[i]->render(this->window);
	}

	this->window->display();
}

// Private Functions
void Game::initWindow() {
	this->view.setSize(Global::WIDTH, Global::HEIGHT);
	this->view.setCenter(Global::WIDTH / 2, Global::HEIGHT / 2);
	this->videoMode = sf::VideoMode(Global::WIN_WIDTH, Global::WIN_HEIGHT);
	this->window = new sf::RenderWindow(this->videoMode, "Chess Defender", sf::Style::Titlebar | sf::Style::Close);
	this->window->setView(this->view);
	this->window->setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width / 2 - Global::WIN_WIDTH / 2, sf::VideoMode::getDesktopMode().height / 2 - Global::WIN_HEIGHT / 2));
	this->window->setKeyRepeatEnabled(false);
}

void Game::initEntities() {
	for (int i = 0; i < this->entities; i++) {
		this->entity[i]->init();
	}
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