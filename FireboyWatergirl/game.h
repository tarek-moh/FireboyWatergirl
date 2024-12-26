#pragma once

#include <SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Window.hpp>
#include<SFML/Network.hpp>
#include<vector>
#include<sstream>
#include<fstream>
#include"gameBoard.h"
using namespace std;


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


	//Controls

	//game logic
	GameBoard gameboard;
	void handle_player_collision(Player&, const sf::Sprite&);
	void handle_border_collision(Player&, const sf::RectangleShape&);
	bool display_Gem(Player&, sf::Sprite&, int,bool[4]);
	int score(bool[4]);
	string formattedTime (float) const;
	void update_remainingTime();
	void store_scores(int,int,float);
	void handle_zone_rules();

	//UI
	sf::Text text;
	sf::Font timer_font;
	sf::Text timer_txt;



	//window related attributes
	sf::RenderWindow* win;
	sf::Event ev;
	sf::VideoMode videoMode;
	sf::Clock clock;
	fstream score_file;
	float delTatime=0.0f;
	float remainingTime=120.f;

};