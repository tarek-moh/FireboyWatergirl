#pragma once
#include <SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Window.hpp>
#include<SFML/Network.hpp>

/// <summary>
/// this class is for creating fireboy & watergirl 
/// in game.cpp we create an area of size 2 where idx[0] contains fireboy and idx[1] contains water girl
/// it contains variables specific to them and functions that they can perform
/// </summary>
class Player {
public: 
	int health; //3 hearts


	//Controls 
	void jump();
	void moveLeft();
	void moveRight();

	//getters
	sf::Vector2f getPos();

private:
	sf::Sprite sprite;
	sf::Vector2f Pos;

};