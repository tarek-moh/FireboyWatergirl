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
		gameboard.fireboy.velocity.y += 981.0f * delTatime; // Gravity
	}
	if (!gameboard.watergirl.isGrounded) {
		gameboard.watergirl.velocity.y += 981.0f  * delTatime; // Gravity
	}
	gameboard.watergirl.sprite.move(gameboard.watergirl.velocity * delTatime);
	gameboard.watergirl.delTajump += gameboard.watergirl.velocity.y * delTatime;
	gameboard.fireboy.sprite.move(gameboard.fireboy.velocity * delTatime);
	gameboard.fireboy.delTajump += gameboard.fireboy.velocity.y * delTatime;
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
	for (int i = 0; i < 11; i++)
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

	//red gems
	for (int i = 0; i < 4; i++)
	{
		if (display_Gem(gameboard.fireboy, gameboard.Rgems[i],i, gameboard.R_already_collided))
		{
			win->draw(gameboard.Rgems[i]);
		}
	}


	//blue gems
	for (int i = 0; i < 4; i++) {
		if (display_Gem(gameboard.watergirl, gameboard.Bgems[i], i,gameboard.B_already_collided))
		{
			win->draw(gameboard.Bgems[i]);
		}
	}


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
	gameboard.fireboy.sprite.setScale(.85, .85);
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

	//initialize background
	gameboard.bg.setSize(sf::Vector2f(1280, 900));
	gameboard.bgT.loadFromFile("assets/images/background2.png");
	gameboard.bgT.setRepeated(1);
	gameboard.bg.setTexture(&gameboard.bgT);
	
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
	gameboard.blocks[0].setPosition(35, 740);
	gameboard.blocks[0].setScale(.7, 1);

	gameboard.blocks[1].setPosition(1170, 735);
	gameboard.blocks[1].setScale(1, 1);

	//floor 1
	gameboard.blocks[4].setPosition(35, 605);
	gameboard.blocks[4].setScale(1, 1);

	gameboard.blocks[5].setPosition(490, 640);
	gameboard.blocks[5].setScale(1.25, 1);


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

	gameboard.blocks[8].setPosition(35, 865);
	gameboard.blocks[8].setScale(2.5, 1);

	gameboard.blocksT.loadFromFile("assets/images/Tb1.png");
	gameboard.blocksT.setRepeated(1);
	for (int i = 0; i < 9; i++)
	{
		gameboard.blocks[i].setTexture(gameboard.blocksT);
	}

	//zone initialization
	gameboard.lavaT.loadFromFile("assets/images/flake.png");
	gameboard.lava.setTexture(gameboard.lavaT);
	gameboard.lava.setPosition(500, 865);

	gameboard.waterT.loadFromFile("assets/images/wlake.png");
	gameboard.water.setTexture(gameboard.waterT);
	gameboard.water.setPosition(900, 865);

	gameboard.gooT.loadFromFile("assets/images/glake.png");
	gameboard.goo.setTexture(gameboard.gooT);
	gameboard.goo.setTextureRect(sf::IntRect(0, 0, 170, 72));
	gameboard.goo.setPosition(520, 640);

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


	//Gems

	gameboard.Blue_gemsT.loadFromFile("assets/images/blue diamond.PNG");
	for (int i = 0; i < 4; i++)
	{
		gameboard.Bgems[i].setTexture(gameboard.Blue_gemsT);
		gameboard.Bgems[i].setScale(0.85, 0.85);
	}
	gameboard.Bgems[0].setPosition(1010, 750);
	gameboard.Bgems[1].setPosition(200, 790);
	gameboard.Bgems[2].setPosition(420, 90);
	gameboard.Bgems[3].setPosition(1200, 790);


	gameboard.Red_gemsT.loadFromFile("assets/images/red diamond.PNG");
	for (int i = 0; i < 4; i++)
	{
		gameboard.Rgems[i].setTexture(gameboard.Red_gemsT);
		gameboard.Rgems[i].setScale(0.9, 0.9);
	}
	gameboard.Rgems[0].setPosition(600, 750);
	gameboard.Rgems[1].setPosition(120, 790);
	gameboard.Rgems[2].setPosition(220, 90);
	gameboard.Rgems[3].setPosition(220, 340);

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
bool Game::is_colliding_from_bottom(const Player& player, const sf::Sprite& block) {
	// Get player and block bounds
	sf::FloatRect char_bounds = player.sprite.getGlobalBounds();
	sf::FloatRect block_bounds = block.getGlobalBounds();

	// Check for intersection
	if (char_bounds.intersects(block_bounds)) {
		// Check if the player is NOT colliding from the bottom
		return char_bounds.top + (char_bounds.height * 0.1) < block_bounds.top;
	}

	// No collision
	return false;
}

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
				player.velocity.y = 200; // Apply upward force for bounce
				player.sprite.setPosition(player.sprite.getPosition().x, block_bounds.top + block_bounds.height + 0.5f);
			}
		}
	}

	//// Reset jump if exceeding max jump height
	//if (fabs(player.delTajump) > player.jumpheight) {
	//	player.delTajump = 0;
	//	player.velocity.y = 200; // Reset downward velocity
	//}

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


bool Game::display_Gem(Player& player, sf::Sprite& Gem, int i,bool already_collided[4])
{
	 
	sf::FloatRect player_bounds = player.sprite.getGlobalBounds();
	sf::FloatRect gems_bounds = Gem.getGlobalBounds();
	if (player_bounds.intersects(gems_bounds)){
		already_collided[i] = true;
		return false;
	}
	else{
		if (!already_collided[i]) {
			return true;
		}
		else{
			return false;
		}
	}
}


int Game::score( bool already_collided[4])
{
	int score=0;
	for (int i = 0; i < 4; i++) {
		if (already_collided[i]){
			score++;
		}
	}
	return score;
}





