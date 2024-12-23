#include "game.h"
#include <iostream>
#include<cstdlib>

Game::Game()
{
	initWin();
	initVars();

}

Game::~Game()
{
	delete this->win;
}

void Game::update()
{
	delTatime = clock.restart().asSeconds();
	gameboard.fireboy.isGrounded = false;
	gameboard.watergirl.isGrounded = false;

	poll();


	// Update players
	if (!gameboard.fireboy.isGrounded) {
		gameboard.fireboy.velocity.y += 900.f * delTatime; // Gravity
	}
	if (!gameboard.watergirl.isGrounded) {
		gameboard.watergirl.velocity.y += 900.f * delTatime; // Gravity
	}
	gameboard.watergirl.sprite.move(gameboard.watergirl.velocity * delTatime);
	//gameboard.watergirl.delTajump += gameboard.watergirl.velocity.y * delTatime;
	gameboard.fireboy.sprite.move(gameboard.fireboy.velocity * delTatime);
	//gameboard.fireboy.delTajump += gameboard.fireboy.velocity.y * delTatime;
	for (int i = 0; i < 15; i++)
	{
		handle_player_collision(gameboard.fireboy, gameboard.blocks[i]);
		handle_player_collision(gameboard.watergirl, gameboard.blocks[i]);
	}

	for (int i = 0; i < 3; i++)
	{
		handle_border_collision(gameboard.fireboy, gameboard.borders[i]);
		handle_border_collision(gameboard.watergirl, gameboard.borders[i]);
	}
	//handle_player_collision(gameboard.fireboy, gameboard.lava);
	//handle_player_collision(gameboard.fireboy, gameboard.water);
	//handle_player_collision(gameboard.fireboy, gameboard.goo);
	//handle_player_collision(gameboard.watergirl, gameboard.lava);
	//handle_player_collision(gameboard.watergirl, gameboard.water);
	//handle_player_collision(gameboard.watergirl, gameboard.goo);

	handle_zone_rules();
	//handling cool down achieving a 3 second cool down
	gameboard.fireboy.cooldown -= delTatime;
	gameboard.watergirl.cooldown -= delTatime;

	//adding flashing effect for cooldown
	if (gameboard.fireboy.cooldown > 0)
	{
		int opacity = static_cast<int>(128 + 127 * std::sin(9*(4- gameboard.fireboy.cooldown)));
		sf::Color currentColor = gameboard.fireboy.sprite.getColor();
		currentColor.a = opacity;
		gameboard.fireboy.sprite.setColor(currentColor);
	}
	else
	{
		sf::Color currentColor = gameboard.fireboy.sprite.getColor();
		currentColor.a = 255;
		gameboard.fireboy.sprite.setColor(currentColor);
	}
	if (gameboard.watergirl.cooldown > 0)
	{
		int opacity = static_cast<int>(128 + 127 * std::sin(9 * (4 - gameboard.fireboy.cooldown)));
		sf::Color currentColor = gameboard.watergirl.sprite.getColor();
		currentColor.a = opacity;
		gameboard.watergirl.sprite.setColor(currentColor);
	}
	else
	{
		sf::Color currentColor = gameboard.watergirl.sprite.getColor();
		currentColor.a = 255;
		gameboard.watergirl.sprite.setColor(currentColor);
	}

}

void Game::render()
{
	this->win->clear(sf::Color(0, 0, 0, 255));
	//draw
	win->draw(gameboard.bg);
	win->draw(gameboard.fireboy.sprite);
	win->draw(gameboard.watergirl.sprite);
	//doors
	win->draw(gameboard.fDoor);
	win->draw(gameboard.wDoor);
	//
	for (int i = 0; i < 15; i++)
	{
		win->draw(gameboard.blocks[i]);
	}
	for (int i = 0; i < 3; i++)
	{
		win->draw(gameboard.borders[i]);
	}
	win->draw(gameboard.lava);
	win->draw(gameboard.water);
	win->draw(gameboard.goo);
	
	//dynamicaly drawing hearts
	int xOffset = 60;

	sf::Vector2f initialPos = gameboard.fireboyHeart.getPosition();
	for (int i = 0; i < gameboard.fireboy.lifes; i++)
	{
		gameboard.fireboyHeart.setPosition(initialPos.x + xOffset * i, initialPos.y);
		win->draw(gameboard.fireboyHeart);
	}
	gameboard.fireboyHeart.setPosition(initialPos.x, initialPos.y);

	initialPos = gameboard.watergirlHeart.getPosition();
	for (int i = 0; i < gameboard.watergirl.lifes; i++)
	{
		gameboard.watergirlHeart.setPosition(initialPos.x + xOffset * i, initialPos.y);
		win->draw(gameboard.watergirlHeart);
	}
	gameboard.watergirlHeart.setPosition(initialPos.x, initialPos.y);

	//display
	this->win->display();

}

bool Game::running()
{
	return this->win->isOpen();
}

void Game::initVars()
{
	initGameboard();

}

void Game::initGameboard()
{
	//fireboy initialization (texture, scale, textureRec, pos)
	if (!gameboard.fireboyT.loadFromFile("assets/sprites/fireboy_sprite.png"))
	{
		std::cout << "couldn't load fireboy\n";
	}
	gameboard.fireboy.sprite.setTexture(gameboard.fireboyT);
	gameboard.fireboy.sprite.setScale(.8, .78);
	gameboard.fireboy.sprite.setTextureRect(sf::IntRect(467, 277, 55, 100));
	gameboard.fireboy.sprite.setPosition(100, 650);

	//watergirl initialization (texture, scale, textureRec, pos)

	if (!gameboard.watergirlT.loadFromFile("assets/sprites/watergirl_sprite.png"))
	{
		std::cout << "couldn't load watergirl\n";
	}
	gameboard.watergirl.sprite.setTexture(gameboard.watergirlT);
	gameboard.watergirl.sprite.setScale(.85, .85);
	gameboard.watergirl.sprite.setTextureRect(sf::IntRect(340, 472, 56, 86));
	gameboard.watergirl.sprite.setPosition(100, 650);

	//initialzie player hearts

	if (!gameboard.fireboyHeartT.loadFromFile("assets/images/fireboyHeart.png"))
	{
		std::cout << "couldn't load fireboy hearts\n";
	}
	gameboard.fireboyHeart.setTexture(gameboard.fireboyHeartT);
	gameboard.fireboyHeart.setPosition(1000.f, 20.f);
	gameboard.fireboyHeart.setScale(2, 2);

	if (!gameboard.watergirlHeartT.loadFromFile("assets/images/watergirlHeart.png"))
	{
		std::cout << "couldn't load watergirl hearts\n";
	}
	gameboard.watergirlHeart.setTexture(gameboard.watergirlHeartT);
	gameboard.watergirlHeart.setPosition(1000.f, 60.f);
	gameboard.watergirlHeart.setScale(2, 2);


	//initialize background


	gameboard.bg.setPosition(0, 0);

	if (!gameboard.bgT.loadFromFile("assets/images/bg.png")) {
		std::cout << "Error loading background texture!" << std::endl;
	}
	gameboard.bg.setTexture(gameboard.bgT);
	sf::Vector2u windowSize = this->win->getSize(); // Get the window size
	float scaleX = (float)windowSize.x / gameboard.bgT.getSize().x;
	float scaleY = (float)windowSize.y / gameboard.bgT.getSize().y;
	gameboard.bg.setScale(scaleX, scaleY);
	//initilize borders

	gameboard.borders[0].setSize(sf::Vector2f(1280.f, 35.f)); //ceiling
	gameboard.borders[1].setSize(sf::Vector2f(35.f, 1280.f));// right wall
	gameboard.borders[2].setSize(sf::Vector2f(35.f, 1280.f)); //left wall
	gameboard.borders[0].setPosition(0, 0);
	gameboard.borders[1].setPosition(1245, 0);
	gameboard.borders[2].setPosition(0, 0);

	gameboard.groundT.setRepeated(1);
	gameboard.groundT.loadFromFile("assets/images/bground1.png");
	gameboard.borders[0].setTexture(&gameboard.groundT); //ceiling
	gameboard.wallsT.loadFromFile("assets/images/bground2.png");
	gameboard.wallsT.setRepeated(1);
	gameboard.borders[1].setTexture(&gameboard.wallsT);
	gameboard.borders[1].setTextureRect(sf::IntRect(105, 120, 35, 900)); // right wall
	gameboard.borders[2].setTexture(&gameboard.wallsT);
	gameboard.borders[2].setTextureRect(sf::IntRect(105, 120, 35, 900)); //left wall

	//floor 0

	gameboard.blocks[8].setPosition(35, 865);
	gameboard.blocks[8].setScale((500.f-35.f)/485.f, 1);

	gameboard.blocks[9].setPosition(743, 865);
	gameboard.blocks[9].setScale(1, 1);

	gameboard.blocks[10].setPosition(500, 880);
	gameboard.blocks[10].setScale(1, 1);


	//floor 1

	gameboard.blocks[0].setPosition(35, 740);
	gameboard.blocks[0].setScale(.7, 1);


	gameboard.blocks[1].setPosition(1170, 755);
	gameboard.blocks[1].setScale(1, 1);

	gameboard.blocks[4].setPosition(35, 610);
	gameboard.blocks[4].setScale(1, 1);

	gameboard.blocks[5].setPosition(490, 645);
	gameboard.blocks[5].setScale(1.25, .5);


	//floor 2

	gameboard.blocks[2].setPosition(760, 435);
	gameboard.blocks[2].setScale(1, 1);

	gameboard.blocks[3].setPosition(175, 400);
	gameboard.blocks[3].setScale(1.25, 1);
	//floor 3
	gameboard.blocks[6].setPosition(35, 270);
	gameboard.blocks[6].setScale(2.2, 0.75);

	//final floor
	gameboard.blocks[7].setPosition(35, 150); 
	gameboard.blocks[7].setScale(1, 1);



	gameboard.blocksT.loadFromFile("assets/images/Tb1.png");
	gameboard.blocksT.setRepeated(1);
	for (int i = 0; i < 15; i++)
	{
		gameboard.blocks[i].setTexture(gameboard.blocksT);
	}

	//zone initialization
	gameboard.lavaT.loadFromFile("assets/images/flake.png");
	gameboard.lava.setTexture(gameboard.lavaT);
	gameboard.lava.setPosition(500, 870);


	gameboard.waterT.loadFromFile("assets/images/wlake.png");
	gameboard.water.setTexture(gameboard.waterT);
	gameboard.water.setPosition(500, 750);
	gameboard.water.setScale(.85, .7);

	gameboard.blocks[11].setPosition(500, 760);
	gameboard.blocks[11].setScale(.49, .7);


	gameboard.gooT.loadFromFile("assets/images/glake1.png");
	gameboard.goo.setTexture(gameboard.gooT);
	gameboard.goo.setTextureRect(sf::IntRect(0, 0, 170, 21));
	gameboard.goo.setPosition(530, 645);
	gameboard.goo.setScale(.90, .8);

	//door initialization
	gameboard.wDoorT.loadFromFile("assets/images/water door1.PNG");
	gameboard.wDoor.setTexture(gameboard.wDoorT);
	gameboard.wDoor.setTextureRect(sf::IntRect(3, 1, 110, 125));
	gameboard.wDoor.setPosition(300, 60);
	gameboard.wDoor.scale(0.75, 0.75);
	// Fire door
	gameboard.fDoorT.loadFromFile("assets/images/fire door1.PNG");
	gameboard.fDoor.setTexture(gameboard.fDoorT);
	gameboard.fDoor.setTextureRect(sf::IntRect(3, 1, 110, 125));
	gameboard.fDoor.setPosition(100, 60);
	gameboard.fDoor.scale(0.75, 0.75);
}

void Game::initWin()
{
	this->videoMode.width = 1280;
	this->videoMode.height = 900;

	this->win = new sf::RenderWindow(this->videoMode, "FireboyWatergirl!");
	this->win->setFramerateLimit(60);

}

void Game::poll()
{
	while (this->win->pollEvent(this->ev)) {
		
		switch (this->ev.type)
		{
		case sf::Event::Closed:
			this->win->close();
			break;
		case sf::Event::KeyPressed:
			if (ev.key.code == sf::Keyboard::Escape)
				this->win->close();
			break;

		}
		if (ev.type == sf::Event::KeyPressed)
		{
			if (ev.key.code == sf::Keyboard::Right)
				gameboard.fireboy.velocity.x = gameboard.fireboy.speed;  //fireboy move right
			

			if (ev.key.code == sf::Keyboard::Left )
				gameboard.fireboy.velocity.x = -gameboard.fireboy.speed;  //fireboy move left

			if (ev.key.code == sf::Keyboard::Up && !gameboard.fireboy.isJumping)
			{
				gameboard.fireboy.velocity.y = -gameboard.fireboy.jumpstrength;

				gameboard.fireboy.isJumping = true;
			}
		}

		if (ev.type == sf::Event::KeyReleased)  //stop movement if key is released
		{
			if (ev.key.code == sf::Keyboard::Right || ev.key.code == sf::Keyboard::Left)
				gameboard.fireboy.velocity.x = 0.0f;    //stop horizontal motion
		}

		if (ev.type == sf::Event::KeyPressed)
		{
			if (ev.key.code == sf::Keyboard::D)
				gameboard.watergirl.velocity.x = gameboard.watergirl.speed;  //watergirl move right

			if (ev.key.code == sf::Keyboard::A )
				gameboard.watergirl.velocity.x = -gameboard.watergirl.speed;  //watergirl move left

			if (ev.key.code == sf::Keyboard::W && !gameboard.watergirl.isJumping)
			{
				gameboard.watergirl.velocity.y = -gameboard.watergirl.jumpstrength;
				gameboard.watergirl.isJumping = true;
			}
		}

		if (ev.type == sf::Event::KeyReleased)  //stop movement if key is released
		{
			if (ev.key.code == sf::Keyboard::A || ev.key.code == sf::Keyboard::D)
				gameboard.watergirl.velocity.x = 0.0f;    //stop horizontal motion
		}
	}
}
//bool Game::is_colliding_from_bottom(const Player& player, const sf::Sprite& block) {
//	// Get player and block bounds
//	sf::FloatRect char_bounds = player.sprite.getGlobalBounds();
//	sf::FloatRect block_bounds = block.getGlobalBounds();
//
//	// Check for intersection
//	if (char_bounds.intersects(block_bounds)) {
//		// Check if the player is NOT colliding from the bottom
//		return char_bounds.top + (char_bounds.height * 0.1) < block_bounds.top;
//	}
//
//	// No collision
//	return false;
//}

void Game::handle_player_collision(Player& player, const sf::Sprite& block)
{
	sf::FloatRect char_bounds = player.sprite.getGlobalBounds();
	sf::FloatRect block_bounds = block.getGlobalBounds();

	if (char_bounds.intersects(block_bounds)) {
		sf::FloatRect overlap;

		// Calculate the overlap
		overlap.height = std::min(char_bounds.top + char_bounds.height, block_bounds.top + block_bounds.height) -
			std::max(char_bounds.top, block_bounds.top);
		overlap.width = std::min(char_bounds.left + char_bounds.width, block_bounds.left + block_bounds.width) -
			std::max(char_bounds.left, block_bounds.left);

		if (overlap.width < overlap.height) {
			// Side collision
			if (char_bounds.left < block_bounds.left) {
				player.sprite.setPosition(block_bounds.left - char_bounds.width - 0.5f, player.sprite.getPosition().y);
				player.velocity.x = 0;
			}
			else {
				player.sprite.setPosition(block_bounds.left + block_bounds.width + 0.5f, player.sprite.getPosition().y);
				player.velocity.x = 0;
			}
		}
		else {
			// Vertical collision
			if (char_bounds.top < block_bounds.top) {
				// Landing on top of the block
				player.isGrounded = true;
				player.isJumping = false;
				player.velocity.y = 0;
				player.sprite.setPosition(player.sprite.getPosition().x, block_bounds.top - char_bounds.height);
			}
			else {
				// Hitting the block from below
				player.velocity.y = 0; // Apply upward force for bounce
				player.sprite.setPosition(player.sprite.getPosition().x, block_bounds.top + block_bounds.height + 0.5f);
			}
		}
	}
}

void Game::handle_zone_rules()
{
	sf::FloatRect fireboyBounds = gameboard.fireboy.sprite.getGlobalBounds();
	sf::FloatRect watergirlBounds = gameboard.watergirl.sprite.getGlobalBounds();
	sf::FloatRect lava_bounds = gameboard.lava.getGlobalBounds();
	sf::FloatRect water_bounds = gameboard.water.getGlobalBounds();
	sf::FloatRect goo_bounds = gameboard.goo.getGlobalBounds();

	if (fireboyBounds.intersects(water_bounds) && gameboard.fireboy.cooldown < 0) {
	
		gameboard.fireboy.cooldown = 3.f;
		gameboard.fireboy.lifes--;
	}
	if (watergirlBounds.intersects(lava_bounds) && gameboard.watergirl.cooldown < 0) {
		gameboard.watergirl.cooldown = 3.f;
		gameboard.watergirl.lifes--;
	}
}


void Game::handle_border_collision(Player& player, const sf::RectangleShape& borders)
{
	sf::FloatRect player_bounds = player.sprite.getGlobalBounds();
	sf::FloatRect borders_bounds = borders.getGlobalBounds();

	if (player_bounds.intersects(borders_bounds))
	{
		float dis = 3.f;
		(player.velocity.x < 0) ? dis *= 1 : dis *= -1;
		player.velocity.x = 0;
		
		player.sprite.move(dis,0);
	}
	
}
