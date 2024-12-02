#pragma once
#include <SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Window.hpp>
#include<SFML/Network.hpp>

class RankingList {

public:
	RankingList();

	//init
	void initVars();
	void initWin();

	//window related functions
	void update();
	void render();
	bool running();

	~RankingList();

private:

	//window related vars
	sf::RenderWindow* win;
	sf::Event ev;
	sf::VideoMode videoMode;
};