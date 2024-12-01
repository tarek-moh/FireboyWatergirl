#include "game.h"
#include <iostream>
#include<cstdlib>

Game::Game()
{
	initWin();
	initVars();

}

Game::~Game()
{
	delete this->win;
}

void Game::update()
{
	poll();

}

void Game::render()
{
	this->win->clear(sf::Color(0, 0, 0, 255));
	//draw


	//display
	this->win->display();

}

bool Game::running()
{
	return this->win->isOpen();
}

void Game::initVars()
{


}

void Game::initWin()
{
	this->videoMode.width = 640;
	this->videoMode.height = 480;

	this->win = new sf::RenderWindow(this->videoMode, "FirstGame!", sf::Style::Default | sf::Style::Titlebar | sf::Style::Close);
	this->win->setFramerateLimit(60);

}

void Game::poll()
{
	while (this->win->pollEvent(this->ev)) {

		switch (this->ev.type)
		{
		case sf::Event::Closed:
			this->win->close();
			break;
		case sf::Event::KeyPressed:
			if (ev.key.code == sf::Keyboard::Escape)
				this->win->close();
			break;
		}
	}
}


void Game::initFont()
{
	//if (this->font.loadFromFile("Fonts/CanvasBags-ZrqB.ttf"))
	//{
	//	std::cout << "fonts successfuly loaded!\n";
	//}
	//else
	//	std::cout << "Warning:: error while loading fonts\n";

}
void Game::initText()
{
	//text.setFont(font);
	//text.setCharacterSize(30);
	//text.setString("NONE");

	//GameOverText.setFont(font);
	//GameOverText.setCharacterSize(70);
	//GameOverText.setString("GAME OVER");
	//GameOverText.setStyle(sf::Text::Bold);
	//GameOverText.setFillColor(sf::Color::Red);

	//sf::Vector2u windowSize = this->win->getSize(); // Replace `window` with your sf::RenderWindow
	//GameOverText.setPosition(
	//	windowSize.x / 2.f - GameOverText.getGlobalBounds().width / 2.f,
	//	windowSize.y / 2.f - GameOverText.getGlobalBounds().height / 2.f
	//);
}

void Game::renderText()
{
	//this->win->draw(text);
}
