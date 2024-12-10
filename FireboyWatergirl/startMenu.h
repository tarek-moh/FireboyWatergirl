#pragma once
#include <SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Window.hpp>
#include<SFML/Network.hpp>
#include<iostream>
#include"game.h"
#include"rankingList.h"
#include"TextFormating.h"

#define STARTMENUMAX 3

class StartMenu
{
public:
	StartMenu(); //class constructor it initializes Text/choice

	//init
	void initVars();
	void initWin();

	//Window related functions
	void update();
	void render();
	bool running();
	
	//functions that change user choice (moveUp: choice--, moveDown: choice++)
	void moveUp();
	void moveDown();

	int startMenuPressed(); //retunr user choice

	~StartMenu();

private:
	int choice; //stores the value the user chooses (0 start, 1 show ranking list, 2 exit)
	sf::Font font;
	sf::Text startMenuText[STARTMENUMAX];  //array of text with the three menu (text) options

	//Window related vars
	sf::RenderWindow* win;
	sf::Event ev;
	sf::VideoMode videoMode;
	
	//backborders
	sf::RectangleShape backborders;
	sf::Texture texture;

};