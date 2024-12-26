#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

struct Player
{
	sf::Sprite sprite;
	sf::Vector2f velocity;
	bool isJumping=false;
	bool isGrounded = true;
	float speed = 240;
	float jumpstrength = 460.f;
	int lifes = 3;
	float cooldown = -1.f;
};

class Elevator // takes positions of elevator and buttons as a parameter
{
public:
	sf::Sprite elevator;
	sf::Sprite button1;
	sf::Sprite button2;

	float speed = 2.0f;
	float targetHeight; // The height at which the elevator stops
	float startHeight;

	float buttonDepth = 10.f; // max distance traveled when pushed
	float b1Y;
	float b2Y;


	Elevator() {};
	Elevator(float eX, float eY, float b1X, float b1Y, float b2X, float b2Y) {
		
		startHeight = eY;
		targetHeight = eY - 100.f;

		this->b1Y = b1Y;
		this->b2Y = b2Y;

		elevator.setPosition(eX, eY); // Starting position
		//elevatorT.loadFromFile("assets/images/Tb1.png");
		//elevator.setTexture(elevatorT);
		elevator.setScale(.3f, .5f);

		//buttonT.loadFromFile("assets/images/buttons_assets.png");
		//button 1
		//button1.setTexture(buttonT);
		button1.setTextureRect(sf::IntRect(518, 29, 78, 29));
		button1.setPosition(b1X, b1Y);
		button1.setScale(.8f, .8f);

		//button 2
		//button2.setTexture(buttonT);
		button2.setTextureRect(sf::IntRect(701, 27, 76, 26));
		button2.setPosition(b2X, b2Y);
		button2.setScale(.8f, .8f);
	}

	bool isOn(sf::FloatRect& playerBounds) {
		playerBounds.height += 3.f;
		return (playerBounds.intersects(button1.getGlobalBounds()) || playerBounds.intersects(button2.getGlobalBounds()));
	}

	void update(const sf::FloatRect& FBbounds, const sf::FloatRect& WGbounds) {
		// Adjust Fireboy and Watergirl bounds for better collision detection
		sf::FloatRect adjustedFBbounds = FBbounds;
		adjustedFBbounds.height += 3.f;
		sf::FloatRect adjustedWGbounds = WGbounds;
		adjustedWGbounds.height += 3.f;

		// Elevator movement logic
		if (isOn(adjustedFBbounds) || isOn(adjustedWGbounds)) {
			// Move elevator up if the button is pressed
			if (elevator.getPosition().y > targetHeight) {
				elevator.move(0, -speed); // Move up
			}
		}
		else {
			// Move elevator down if the button is not pressed
			if (elevator.getPosition().y < startHeight) {
				elevator.move(0, speed); // Move down
			}
		}

		// Button animation logic
		if (button1.getGlobalBounds().intersects(adjustedFBbounds) || button1.getGlobalBounds().intersects(adjustedWGbounds)) {
			// Animate button downwards if pressed
			if (button1.getPosition().y < b1Y + 5.f) { // Pressed position threshold
				button1.move(0, 1.f); // Move down slightly
			}
		}
		else {
			// Return button to original position if released
			if (button1.getPosition().y > b1Y) { // Original position
				button1.move(0, -1.f); // Move up slightly
			}
		}
		if (button2.getGlobalBounds().intersects(adjustedFBbounds) || button2.getGlobalBounds().intersects(adjustedWGbounds)) {
			// Animate button downwards if pressed
			if (button2.getPosition().y < b2Y + 5.f) { // Pressed position threshold
				button2.move(0, 1.f); // Move down slightly
			}
		}
		else {
			// Return button to original position if released
			if (button2.getPosition().y > b2Y) { // Original position
				button2.move(0, -1.f); // Move up slightly
			}
		}
	}

	void draw(sf::RenderWindow& window) {
		window.draw(elevator);
		window.draw(button1);
		window.draw(button2);

	}

};


struct GameBoard 
{
	// ---------------------------------------------map---------------------------------
	sf::Sprite blocks[15]; 
	sf::RectangleShape borders[3];
	sf::RectangleShape bg; //background

	// ---------------------------------------------traps---------------------------------
	sf::Sprite lava;
	sf::Sprite water;
	sf::Sprite goo;
	sf::Sprite rope;
	sf::Sprite movingTrap;
	// Create the platform

	// Initial angular settings
	float angle = 0.f; // Initial angle in degrees
	float angularVelocity = 0.0f;
	float angularAcceleration = 0.0f;

	// ---------------------------------------------doors--------------------------------
	sf::Sprite wDoor;
	sf::Sprite fDoor;

	// ---------------------------------------------Characters---------------------------
	Player fireboy;
	Player watergirl;
	sf::Sprite fireboyHeart;
	sf::Sprite watergirlHeart;
	// ---------------------------------------------elevators---------------------------
	Elevator elevator[2];
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
	sf::Texture ropeT;
	sf::Texture movingTrapT;
	sf::Texture buttonT;
	sf::Texture elevatorT;
	sf::Texture fireVictoryT;
	sf::Texture waterVictoryT;
};