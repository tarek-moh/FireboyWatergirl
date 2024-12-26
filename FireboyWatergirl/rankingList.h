#pragma once
#include <SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Window.hpp>
#include<SFML/Network.hpp>
#include<iostream>
#include<iomanip>
#include<fstream>
#include <vector>
#include <tuple> //for std::tuple
#include"game.h"
using namespace std;

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
	float evaluated_weight(int,int,float);
	void bubble_sorting(vector<tuple<int, int, float>>&);
	void read_file();
	string formattedTime(float) const;


private:
	//window related vars
	sf::RenderWindow* win;
	sf::Event ev;
	sf::VideoMode videoMode;
	fstream file;

	//Textures
	sf::Texture winbackgrT;
	sf::Texture headlineT;
	sf::Texture listT;

	//sprites
	sf::Sprite headline;
	sf::Sprite winbackgr;
	sf::Sprite list;

	//Texts and Fonts
	sf::Text headlineTxt;
	sf::Text headers_txt[4];
	sf::Text details_txt[10][4];
	sf::Font font;
	
	vector<tuple<int, int, float>>vec;
	int watergirl_score, fireboy_score;
	float remainingTime;


};