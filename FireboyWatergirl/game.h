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


	//Controls

	//game logic
	GameBoard gameboard;
	void handle_trap_collision();
	void handle_zone_rules();
	void handle_player_collision(Player& player, const sf::FloatRect& block);
	void handle_border_collision(sf::Sprite& player, const sf::FloatRect& borders_bounds);
	void handle_animation();
	void victory();
	void defeat();
	bool isVictory = 0;
	bool isDefeat = 0;

	//UI
	sf::Font font;
	sf::Text text;

	//void initFont();
	//void initText();
	//void renderText();


	//window related attributes
	sf::RenderWindow* win;
	sf::Event ev;
	sf::VideoMode videoMode;
	// ---------------------------------------------Animation---------------------------------
	sf::Clock animationClock; //in order to handle time between frames in animation
	int currentFrame = 0;
	float frameDuration = 0.1f;
	int currentFframe = 0;//tracks current frame for fire door
	int currentWframe = 0;//tracks current frame for water door

	int currentFrameVictory=0;
	// ----------------------------------------time---------------------------
	sf::Clock clock;
	float delTatime=0.0f;
	float collisionTime=0.0f;

};