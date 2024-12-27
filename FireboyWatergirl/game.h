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
	void handle_trap_collision();
	void handle_player_collision(Player&, const sf::Sprite&);
	void handle_border_collision(Player&, const sf::RectangleShape&);
	bool display_Gem(Player&, sf::Sprite&, int,bool[4]);
	int score(bool[4]);
	string formattedTime (float) const;
	void update_remainingTime();
	void store_scores(int,int,float);
	void handle_zone_rules();
	void handle_player_collision(Player& player, const sf::FloatRect& block);
	void handle_border_collision(sf::Sprite& player, const sf::FloatRect& borders_bounds);
	void handle_animation();
	void victory();
	void defeat();
	bool isVictory = 0;
	bool isDefeat = 0;

	//UI
	sf::Text text;
	sf::Font timer_font;
	sf::Text timer_txt;
 


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
	fstream score_file;
	float delTatime=0.0f;
	float remainingTime=120.f;
	float collisionTime=0.0f;
	bool stored = false;
	sf::RectangleShape popupScreen;
	sf::Text gamecondition;
	sf::Texture rgT;
	sf::Texture bgT;
	sf::Sprite rg;
	sf::Sprite bg;
	sf::Text victorytxt[3][3];


};