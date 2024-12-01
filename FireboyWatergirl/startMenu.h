#pragma once
#include <SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Window.hpp>
#include<SFML/Network.hpp>

class StartMenu
{
public:
	StartMenu(float width, float height); //class constructor it initializes 

	//functions that change user choice (moveUp: choice--, moveDown: choice++)
	void moveUp();
	void moveDown();

	int startMenuPressed(); //retunr user choice

	~StartMenu();

private:
	int choice; //stores the value the user chooses (0 start, 1 show ranking list, 2 exit)
	sf::Font font;
	sf::Text startMenuText[3];  //array of text with the three menu (text) options

};