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
	bool isGrounded = true;
	float speed = 240;
	//const float jumpheight = 70;
	//float delTajump = 0;
	float jumpstrength = 460.f;
	int lifes = 3;
	float cooldown = -1.f;
};

struct GameBoard 
{
	Player fireboy;
	Player watergirl;
	sf::Sprite blocks[15]; 
	sf::RectangleShape borders[3];
	sf::Sprite bg; //background
	sf::Sprite lava;
	sf::Sprite water;
	sf::Sprite goo;
	sf::Sprite wDoor;
	sf::Sprite fDoor;
	sf::Sprite fireboyHeart;
	sf::Sprite watergirlHeart;

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
	sf::Texture fireboyHeartT;
	sf::Texture watergirlHeartT;
};