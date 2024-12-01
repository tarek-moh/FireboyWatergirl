#pragma once

#include <SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Window.hpp>
#include<SFML/Network.hpp>
#include<vector>
#include<sstream>


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
	void initWin();


	//event polling
	void poll();

	//controls


	//game logic

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

	//controls
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;


};