#pragma once

#include <SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Window.hpp>
#include<SFML/Network.hpp>
#include<vector>
#include<sstream>
#include"gameBoard.h"

class Game
{
public:
	Game();
	virtual ~Game();

	void update();
	void render();
	bool running();
private:
	//init
	void initVars();
	void initGameboard();
	void initWin();


	//event polling
	void poll();

	bool is_colliding_from_bottom(const Player& player, const sf::Sprite& block);

	//Controls

	//game logic
	GameBoard gameboard;
	void handle_player_collision(Player&, const sf::Sprite&);
	void handle_border_collision(Player&, const sf::RectangleShape&);

	//UI
	sf::Font font;
	sf::Text text;

	void initFont();
	void initText();
	void renderText();


	//window related attributes
	sf::RenderWindow* win;
	sf::Event ev;
	sf::VideoMode videoMode;
	sf::Clock clock;
	float delTatime=0.0f;

};