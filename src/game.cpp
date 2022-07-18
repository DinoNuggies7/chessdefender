#include "game.h"

// Public Functions
Game::Game() {
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

	this->player.update(this->dt);
}

void Game::render() {
	this->window->clear(sf::Color::Black);

	this->player.render(this->window);

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
	this->player.init("assets/player.png");
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