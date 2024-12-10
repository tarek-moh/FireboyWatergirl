#pragma once
#include <SFML/Graphics.hpp>

//#include<SFML/System.hpp>
//#include<SFML/Window.hpp>
//#include<SFML/Network.hpp>

struct Player
{
	sf::Sprite sprite;
	sf::Vector2f velocity;
	bool isJumping=false;
	float speed = 150;
	const float jumpheight = 100;
	float delTajump = 0;
	float jumpstrength = 300;

};

struct GameBoard 
{
	Player fireboy;
	Player watergirl;
	sf::Sprite blocks[15]; // first level has 8 blocks and floor at [8]
	sf::RectangleShape borders[2];
	sf::RectangleShape bg; //background
	sf::Sprite lava;
	sf::Sprite water;
	sf::Sprite goo;
	sf::Sprite wDoor;
	sf::Sprite fDoor;

	//textures
	sf::Texture fireboyT;
	sf::Texture watergirlT;
	sf::Texture	blocksT;
	sf::Texture wallsT;
	sf::Texture groundT;
	sf::Texture bgT; //background 
	sf::Texture lavaT;
	sf::Texture waterT;
	sf::Texture gooT;
	sf::Texture wDoorT;
	sf::Texture fDoorT;
};