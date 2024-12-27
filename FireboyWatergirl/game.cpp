#include "game.h"
#include <iostream>
#include<cstdlib>
#include <iomanip>
#include<cmath>

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
	
	gameboard.fireboy.isGrounded = false;
	gameboard.watergirl.isGrounded = false;

	update_remainingTime();// update remaining time each frame
	if (isVictory) {
		victorytxt[0][1].setString(to_string(score(gameboard.B_already_collided)));
		victorytxt[1][1].setString(to_string(score(gameboard.R_already_collided)));
	}
	if (isVictory&&!stored) {   //store scores in case of winning 
		store_scores(score(gameboard.B_already_collided), score(gameboard.R_already_collided), this->remainingTime);
		stored = true;
	}

	poll();

	// Update players
	if (!gameboard.fireboy.isGrounded) {
		gameboard.fireboy.velocity.y += 900.f * delTatime; // Gravity
	}
	if (!gameboard.watergirl.isGrounded) {
		gameboard.watergirl.velocity.y += 900.f * delTatime; // Gravity
	}
	gameboard.watergirl.sprite.move(gameboard.watergirl.velocity * delTatime);
	gameboard.fireboy.sprite.move(gameboard.fireboy.velocity * delTatime);

	for (int i = 0; i < 15; i++)
	{
		handle_player_collision(gameboard.fireboy, gameboard.blocks[i].getGlobalBounds());
		handle_player_collision(gameboard.watergirl, gameboard.blocks[i].getGlobalBounds());
	}
	handle_player_collision(gameboard.fireboy, gameboard.movingTrap.getGlobalBounds());
	handle_player_collision(gameboard.watergirl, gameboard.movingTrap.getGlobalBounds());
	
	for (int i = 0; i < 2; i++)
	{
		handle_player_collision(gameboard.fireboy, gameboard.elevator[i].elevator.getGlobalBounds());
		handle_player_collision(gameboard.watergirl, gameboard.elevator[i].elevator.getGlobalBounds());
		handle_player_collision(gameboard.fireboy, gameboard.elevator[i].button1.getGlobalBounds());
		handle_player_collision(gameboard.fireboy, gameboard.elevator[i].button2.getGlobalBounds());
		handle_player_collision(gameboard.watergirl, gameboard.elevator[i].button1.getGlobalBounds());
		handle_player_collision(gameboard.watergirl, gameboard.elevator[i].button2.getGlobalBounds());

	}


	for (int i = 0; i < 3; i++)
	{
		handle_border_collision(gameboard.fireboy.sprite, gameboard.borders[i].getGlobalBounds());
		handle_border_collision(gameboard.watergirl.sprite, gameboard.borders[i].getGlobalBounds());
	}

	handle_trap_collision();//for adding impulse on the trap

	handle_zone_rules();
	//handling cool down achieving a 3 second cool down
	gameboard.fireboy.cooldown -= delTatime;
	gameboard.watergirl.cooldown -= delTatime;

	//adding flashing effect for cooldown
	if (gameboard.fireboy.cooldown > 0)
	{
		int opacity = static_cast<int>(170 + 85 * std::sin(9* (.698 - gameboard.fireboy.cooldown)));
		sf::Color currentColor = gameboard.fireboy.sprite.getColor();
		currentColor.a = opacity;
		gameboard.fireboy.sprite.setColor(currentColor);
		//hearts animation
		currentColor = gameboard.fireboyHeart.getColor();
		currentColor.a = static_cast<int>(170 + 85 * std::sin(9 * (.698 - gameboard.fireboy.cooldown))); // full cycle is 6.28
		gameboard.fireboyHeart.setColor(currentColor);
	}
	else
	{
		sf::Color currentColor = gameboard.fireboy.sprite.getColor();
		currentColor.a = 255;
		gameboard.fireboy.sprite.setColor(currentColor);
		currentColor = gameboard.fireboyHeart.getColor();
		currentColor.a = 255;
		gameboard.fireboyHeart.setColor(currentColor);

	}
	if (gameboard.watergirl.cooldown > 0)
	{
		int opacity = static_cast<int>(170 + 85 * std::sin(9 * (.698 - gameboard.watergirl.cooldown)));
		sf::Color currentColor = gameboard.watergirl.sprite.getColor();
		currentColor.a = opacity;
		gameboard.watergirl.sprite.setColor(currentColor);
		//hearts animation
		currentColor = gameboard.watergirlHeart.getColor();
		currentColor.a = static_cast<int>(170 + 85 * std::sin(9 * (.698 - gameboard.watergirl.cooldown))); // full cycle is 6.28
		gameboard.watergirlHeart.setColor(currentColor);
	}
	else
	{
		sf::Color currentColor = gameboard.watergirl.sprite.getColor();
		currentColor.a = 255;
		gameboard.watergirl.sprite.setColor(currentColor);
		currentColor = gameboard.watergirlHeart.getColor();
		currentColor.a = 255;
		gameboard.watergirlHeart.setColor(currentColor);
	}

	handle_animation();

	//------------------------------------------------------------- moving Trap ----------------------------------------
	const float GRAVITY = 9.8f;  // Acceleration due to gravity
	const float LENGTH = gameboard.rope.getGlobalBounds().height; // Length of the rope
	const float ANGLE_DAMPING = 0.99f; // Damping to reduce swinging over time

	gameboard.angularAcceleration = -(GRAVITY / LENGTH) * std::sin(gameboard.angle * 3.14159f / 180.0f); // Convert to radians
	gameboard.angularVelocity += gameboard.angularAcceleration;
	gameboard.angularVelocity *= ANGLE_DAMPING; // Apply damping
	gameboard.angle += gameboard.angularVelocity;

	// Update rope rotation
	gameboard.rope.setRotation(gameboard.angle + 90);
	//moving trap platform

	float ropeBottomX = gameboard.rope.getPosition().x + LENGTH * -std::sin(gameboard.angle * 3.14159f / 180.0f);
	float ropeBottomY = gameboard.rope.getPosition().y + LENGTH * std::cos(gameboard.angle * 3.14159f / 180.0f);

	// Attach platform to the bottom of the rope
	gameboard.movingTrap.setPosition(ropeBottomX - gameboard.movingTrap.getGlobalBounds().width / 2.0f, ropeBottomY);
	//------------------------------------------------------------- elevator ----------------------------------------
	gameboard.elevator[0].update(gameboard.fireboy.sprite.getGlobalBounds(), gameboard.watergirl.sprite.getGlobalBounds());
	gameboard.elevator[1].update(gameboard.fireboy.sprite.getGlobalBounds(), gameboard.watergirl.sprite.getGlobalBounds());

	//-------------------------------------------------------- VICTORY --------------------------------------------
	
	sf::FloatRect FBbounds = gameboard.fireboy.sprite.getGlobalBounds();
	sf::FloatRect WGbounds = gameboard.watergirl.sprite.getGlobalBounds();
	sf::FloatRect fDoor = gameboard.fDoor.getGlobalBounds();
	sf::FloatRect wDoor = gameboard.wDoor.getGlobalBounds();

	if (currentFframe == 17 && currentWframe == 18)
	{

		//gameboard.fireVictoryT.loadFromFile("assets/images/CharAssets.png");
		//gameboard.fireboy.sprite.setTexture(gameboard.fireVictoryT);
		//gameboard.waterVictoryT.loadFromFile("assets/images/CharAssets.png");
		//gameboard.watergirl.sprite.setTexture(gameboard.fireVictoryT);

		isVictory = 1;
		std::cout << "VICTORY! ";//debugging
	}

	//-------------------------------------------------------- DEFEAT --------------------------------------------
	if (gameboard.fireboy.lifes <= 0 || gameboard.watergirl.lifes <= 0 || fabs(remainingTime - 0) < 1e-9) {
		isDefeat = 1;
		cout << "defeated";
	}

	//update time each frame
	delTatime = clock.restart().asSeconds(); 
}

void Game::render()
{
	this->win->clear(sf::Color(0, 0, 0, 255));
	//draw
	win->draw(gameboard.bg);

	//doors
	win->draw(gameboard.fDoor);
	win->draw(gameboard.wDoor);


	//players
	if(gameboard.fireboy.lifes > 0)
		win->draw(gameboard.fireboy.sprite);
	if (gameboard.watergirl.lifes > 0)
		win->draw(gameboard.watergirl.sprite);

	//traps
	win->draw(gameboard.rope);
	win->draw(gameboard.movingTrap);

	//elevators
	for (int i = 0; i < 2; i++)
	{
		gameboard.elevator[i].draw(*win);
	}


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

	//timer backgroud
	win->draw(gameboard.Timerbackg);
	//timer text
	win->draw(timer_txt);

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

	// --------------------------------------------------------VICTORY------------------------------------
	if (isVictory)
	{
		win->draw(popupScreen);  //draw screen
		win->draw(gamecondition); //draw "Winner"
		win->draw(rg);
		win->draw(bg);

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				win->draw(victorytxt[i][j]);
				win->draw(Mainmenu);
				win->draw(menuText);
			}
		}
	}

	
	if (isDefeat)
	{
		win->draw(popupScreen);
		win->draw(Mainmenu);
		win->draw(menuText);
		win->draw(defeatText);
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
	gameboard.fireboy.sprite.setScale(.8, .78);
	gameboard.fireboy.sprite.setTextureRect(sf::IntRect(467, 395, 55, 100));
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

	if (!gameboard.bgT.loadFromFile("assets/images/background2.png")) {
		std::cout << "Error loading background texture!" << std::endl;
	}
	gameboard.bgT.setRepeated(1);
	gameboard.bgT.setSmooth(1);
	gameboard.bg.setSize(sf::Vector2f(1280.f, 900.f));
	gameboard.bg.setTexture(&gameboard.bgT);
	gameboard.bg.setTextureRect(sf::IntRect(0, 0, gameboard.bg.getSize().x, gameboard.bg.getSize().y));

	//-------------------------------------------------------- BORDER INIT --------------------------------------------

	gameboard.borders[0].setSize(sf::Vector2f(1280.f, 35.f)); //ceiling
	gameboard.borders[1].setSize(sf::Vector2f(35.f, 1280.f));// right wall
	gameboard.borders[2].setSize(sf::Vector2f(35.f, 1280.f)); //left wall
	gameboard.borders[0].setPosition(0, 0);
	gameboard.borders[1].setPosition(1245, 0);
	gameboard.borders[2].setPosition(0, 0);

	gameboard.groundT.setRepeated(1);
	gameboard.groundT.loadFromFile("assets/images/bground1.png");
	gameboard.borders[0].setTexture(&gameboard.groundT); //ceiling
	gameboard.borders[0].setTextureRect(sf::IntRect(0, 0, gameboard.borders[0].getSize().x, gameboard.borders[0].getSize().y));
	gameboard.wallsT.loadFromFile("assets/images/bground2.png");

	gameboard.wallsT.setRepeated(1);
	gameboard.borders[1].setTexture(&gameboard.wallsT);
	gameboard.borders[1].setTextureRect(sf::IntRect(105, 120, 35, 900)); // right wall

	gameboard.borders[2].setTexture(&gameboard.wallsT);
	gameboard.borders[2].setTextureRect(sf::IntRect(105, 120, 35, 900)); //left wall

	//-------------------------------------------------------- floors INIT --------------------------------------------
	//floor 0

	gameboard.blocks[8].setPosition(35, 865);
	gameboard.blocks[8].setScale((500.f - 35.f) / 485.f, 1);

	gameboard.blocks[9].setPosition(743, 865);
	gameboard.blocks[9].setScale(1.3, 1);

	gameboard.blocks[10].setPosition(500, 880);
	gameboard.blocks[10].setScale(.5, 1);


	//floor 1

	gameboard.blocks[0].setPosition(35, 740);
	gameboard.blocks[0].setScale(.7, 1);


	gameboard.blocks[1].setPosition(1170, 755);
	gameboard.blocks[1].setScale(1, 1);


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
	gameboard.goo.setPosition(500, 638);
	gameboard.goo.setScale(2, .9);

	gameboard.blocks[4].setPosition(35, 640);
	gameboard.blocks[4].setScale(2.2, 1);
	// --------------------------------------------- moving trap ---------------------------------
	gameboard.ropeT.loadFromFile("assets/images/rope1.png");
	gameboard.rope.setTexture(gameboard.ropeT);
	gameboard.rope.setScale(.7f, .5f);
	sf::FloatRect ropeBounds = gameboard.rope.getGlobalBounds();
	gameboard.rope.setOrigin(0, ropeBounds.height / 2);
	gameboard.rope.setPosition(660, 435);
	gameboard.rope.rotate(90);

	gameboard.movingTrap.setTexture(gameboard.blocksT);
	gameboard.movingTrap.setScale(.3, .5);
	// --------------------------------------------- elevator ---------------------------------
	gameboard.elevatorT.loadFromFile("assets/images/Tb1.png");
	gameboard.buttonT.loadFromFile("assets/images/buttons_assets.png");

	gameboard.elevator[0] = Elevator(35.f, 610.f, 300.f, 620.f, 354.f, 380.f);
	gameboard.elevator[1] = Elevator(1100.f, 410.f, 800.f, 415.f, 1000.f, 250.f);
	for (int i = 0; i < 2; i++)
	{
		gameboard.elevator[i].elevator.setTexture(gameboard.elevatorT);
		gameboard.elevator[i].button1.setTexture(gameboard.buttonT);
		gameboard.elevator[i].button2.setTexture(gameboard.buttonT);

	}

	//gameboard.blocks[12].setPosition(490, 650);
	//gameboard.blocks[12].setScale(.3f, .6f);
	//gameboard.blocks[13].setPosition(490, 640);
	//gameboard.blocks[13].setScale(.13f, .7f);

	//door initialization
	gameboard.wDoorT.loadFromFile("assets/images/water door1.PNG");
	gameboard.wDoor.setTexture(gameboard.wDoorT);
	gameboard.wDoor.setTextureRect(sf::IntRect(3, 1, 112, 120));
	gameboard.wDoor.setPosition(300, 60);
	//gameboard.wDoor.setPosition(300, 700);
	gameboard.wDoor.scale(0.75, 0.75);
	// Fire door
	gameboard.fDoorT.loadFromFile("assets/images/fire door1.PNG");
	gameboard.fDoor.setTexture(gameboard.fDoorT);
	gameboard.fDoor.setTextureRect(sf::IntRect(3, 1, 112, 120));
	gameboard.fDoor.setPosition(100, 60);
	//gameboard.fDoor.setPosition(100, 700);
	gameboard.fDoor.scale(0.75, 0.75);


	// Blue Gems
	gameboard.Blue_gemsT.loadFromFile("assets/images/blue diamond.PNG");
	for (int i = 0; i < 4; i++)
	{
		gameboard.Bgems[i].setTexture(gameboard.Blue_gemsT);
		gameboard.Bgems[i].setScale(0.85, 0.85);
	}
	gameboard.Bgems[0].setPosition(600, 705);
	gameboard.Bgems[1].setPosition(200, 790);
	gameboard.Bgems[2].setPosition(910, 520);
	gameboard.Bgems[3].setPosition(420, 90);

	//Red Gems
	gameboard.Red_gemsT.loadFromFile("assets/images/red diamond.PNG");
	for (int i = 0; i < 4; i++)
	{
		gameboard.Rgems[i].setTexture(gameboard.Red_gemsT);
		gameboard.Rgems[i].setScale(0.9, 0.9);
	}
	gameboard.Rgems[0].setPosition(600, 805);
	gameboard.Rgems[1].setPosition(120, 790);
	gameboard.Rgems[2].setPosition(220, 90);
	gameboard.Rgems[3].setPosition(360, 340);

	//Timer text and background setup
	if (!timer_font.loadFromFile("assets/fonts/Roboto-Regular.ttf")) {
		cout << "Error loading font!";
	}
	timer_txt.setFont(timer_font);
	timer_txt.setCharacterSize(40);
	timer_txt.setFillColor(sf::Color::White);
	timer_txt.setPosition(600, 0);

	gameboard.TimerbackgT.loadFromFile("assets/images/timer  background.PNG");
	gameboard.Timerbackg.setTexture(gameboard.TimerbackgT);
	gameboard.Timerbackg.setScale(1, 0.8);
	gameboard.Timerbackg.setPosition(523, 0);
	// --------------------------------------------- game over ------------------------------------
	popupScreen.setFillColor(sf::Color(0, 0, 0, 140));
	popupScreen.setSize(sf::Vector2f(1000, 700));
	popupScreen.setPosition(140.f, 100.f);

	defeatText.setFont(timer_font);
	defeatText.setString("Game over");
	defeatText.setCharacterSize(70);
	defeatText.setFillColor(sf::Color::White);
	defeatText.setStyle(sf::Text::Bold);
	defeatText.setPosition(450, 400);
	defeatText.setOutlineColor(sf::Color::Black);
	defeatText.setOutlineThickness(8.f);

	//for main menu button
	menuText.setFont(timer_font);
	menuText.setString("Main menu");
	menuText.setCharacterSize(30);
	menuText.setFillColor(sf::Color(0,0,0,190));
	menuText.setStyle(sf::Text::Bold);
	//menuText.setOutlineColor();
	//menuText.setOutlineThickness(1.f);
	menuText.setPosition(565, 595);

	Mainmenu.setSize(sf::Vector2f(200.f, 50.f));
	Mainmenu.setFillColor(sf::Color(255,255,255,190));
	Mainmenu.setOutlineColor(sf::Color::Black);
	Mainmenu.setOutlineThickness(1.f);
	Mainmenu.setPosition(540, 590);

		//"Winner" text
		gamecondition.setFont(timer_font);
		gamecondition.setFillColor(sf::Color::White);
		gamecondition.setPosition(sf::Vector2f(560, 100)); //set the pos right
		gamecondition.setString("Victory");
		gamecondition.setCharacterSize(60);

		rgT.loadFromFile("assets/images/red diamond.PNG");
		rg.setTexture(rgT);
		rg.setScale(1.3, 1.3);
		rg.setPosition(300, 310);

		bgT.loadFromFile("assets/images/blue diamond.PNG");
		bg.setTexture(bgT);
		bg.setScale(1.3, 1.3);
		bg.setPosition(300, 410);

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				victorytxt[i][j].setFont(timer_font);
			}
			victorytxt[0][0].setPosition(400, 310);
			victorytxt[1][0].setPosition(400, 412); 

			victorytxt[i][0].setString("X");
			victorytxt[i][0].setCharacterSize(50);


			victorytxt[0][1].setPosition(500, 415);
			victorytxt[1][1].setPosition(500, 315);
			victorytxt[i][1].setCharacterSize(40);
			victorytxt[i][1].setCharacterSize(40);

			victorytxt[0][2].setString("Red Gems");
			victorytxt[1][2].setString("Blue Gems");
			victorytxt[0][2].setPosition(580, 320);
			victorytxt[1][2].setPosition(580, 420);

			
			victorytxt[2][0].setPosition(280, 500);
			victorytxt[2][1].setString("Elapsed Time");
			victorytxt[2][1].setPosition(480, 508);

		
		}

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
		if(!isVictory && !isDefeat)
		{
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
				{
					gameboard.fireboy.velocity.x = gameboard.fireboy.speed;  //fireboy move right
					gameboard.fireboy.sprite.setTextureRect(sf::IntRect(12, 0, 65, 74));
				}
				if (ev.key.code == sf::Keyboard::Left)
				{
					gameboard.fireboy.velocity.x = -gameboard.fireboy.speed;  //fireboy move left
					gameboard.fireboy.sprite.setTextureRect(sf::IntRect(12, 102, 65, 74));

				}

				if (ev.key.code == sf::Keyboard::Up && !gameboard.fireboy.isJumping)
				{
					gameboard.fireboy.velocity.y = -gameboard.fireboy.jumpstrength;

					gameboard.fireboy.isJumping = true;
				}

				if (ev.key.code == sf::Keyboard::D)
				{
					gameboard.watergirl.velocity.x = gameboard.watergirl.speed;  //watergirl move right
					gameboard.watergirl.sprite.setTextureRect(sf::IntRect(12, 123, 87, 75));
				}

				if (ev.key.code == sf::Keyboard::A)
				{
					gameboard.watergirl.velocity.x = -gameboard.watergirl.speed;  //watergirl move left
					gameboard.watergirl.sprite.setTextureRect(sf::IntRect(12, 22, 87, 75));
				}

				if (ev.key.code == sf::Keyboard::W && !gameboard.watergirl.isJumping)
				{
					gameboard.watergirl.velocity.y = -gameboard.watergirl.jumpstrength;
					gameboard.watergirl.isJumping = true;
				}
			}

			if (ev.type == sf::Event::KeyReleased)  //stop movement if key is released
			{
				if (ev.key.code == sf::Keyboard::Right || ev.key.code == sf::Keyboard::Left)
				{
					gameboard.fireboy.velocity.x = 0.0f;
					gameboard.fireboy.sprite.setTextureRect(sf::IntRect(467, 395, 55, 100));
				}
				if (ev.key.code == sf::Keyboard::A || ev.key.code == sf::Keyboard::D)
				{
					gameboard.watergirl.velocity.x = 0.0f;    //stop horizontal motion
					gameboard.watergirl.sprite.setTextureRect(sf::IntRect(340, 472, 56, 86));
				}
			}
		}

		//-------------------------------------------gameover-----------------------------------
		if (isDefeat || isVictory)
		{
			switch (ev.type) {
			case sf::Event::Closed:
				this->win->close();
				break;

			case sf::Event::MouseButtonPressed:
				if (ev.mouseButton.button == sf::Mouse::Left) {
					sf::Vector2i mousePos = sf::Mouse::getPosition(*this->win);

					if (Mainmenu.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
						this->win->close();
					}
				}
				break;

			default:
				break;
			}
		}
	}
}


void Game::handle_player_collision(Player& player, const sf::FloatRect& block)
{
	sf::FloatRect char_bounds = player.sprite.getGlobalBounds();
	sf::FloatRect block_bounds = block;

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

void Game::handle_trap_collision()
{
	sf::FloatRect FBbounds = gameboard.fireboy.sprite.getGlobalBounds();
	sf::FloatRect WGbounds = gameboard.watergirl.sprite.getGlobalBounds();
	sf::FloatRect trapbounds = gameboard.movingTrap.getGlobalBounds();

	float trapMidpoint = trapbounds.getPosition().x + trapbounds.width / 2;

	if (FBbounds.intersects(trapbounds)) {
		if (gameboard.fireboy.sprite.getPosition().x < trapMidpoint)
		{
			gameboard.angularVelocity -= .3;
		}
		else
		{
			gameboard.angularVelocity += .3;

		}
	}

	WGbounds.height += .1;
	if (WGbounds.intersects(trapbounds) ) {
		std::cout << "Watergirl intersects trap" << std::endl;  // Debugging line
		if (gameboard.watergirl.sprite.getPosition().x < trapMidpoint)
		{
			gameboard.angularVelocity -= .04;
		}
		else
		{
			gameboard.angularVelocity += .04;

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

	sf::FloatRect adjustedGooBounds = goo_bounds;
	adjustedGooBounds.left += 30.f; // Expand the left side by 5px
	adjustedGooBounds.width -= 60.f; // Expand the width by 10px (5px on each side)
	if (watergirlBounds.intersects(adjustedGooBounds) && gameboard.watergirl.cooldown < 0) {

		gameboard.watergirl.cooldown = 3.f;
		gameboard.watergirl.lifes--;
	}
	if (fireboyBounds.intersects(adjustedGooBounds) && gameboard.fireboy.cooldown < 0) {

		gameboard.fireboy.cooldown = 3.f;
		gameboard.fireboy.lifes--;
	}
}

void Game::handle_border_collision(sf::Sprite& player, const sf::FloatRect& borders_bounds)
{
	sf::FloatRect player_bounds = player.getGlobalBounds();

	if (player_bounds.intersects(borders_bounds))
	{
		float dis = 3.f;
		(player.getPosition().x < borders_bounds.left + borders_bounds.width / 2) ? dis *= -1 : dis *= 1;

		// Stop horizontal velocity and move the player slightly to resolve the collision
		player.move(dis, 0);
	}
}

void Game::handle_animation()
{
	float frameWidth, frameHeight;
	int frameCount = 5;
	if (animationClock.getElapsedTime().asSeconds() > frameDuration) {

		//Goo animation
		frameWidth = 163.9;
		frameHeight = 21;
		gameboard.goo.setTextureRect(sf::IntRect(currentFrame * frameWidth, 0, frameWidth, frameHeight));
		//watergirl animation
		frameWidth = 56;
		frameHeight = 108;
		sf::IntRect WGTextRect = gameboard.watergirl.sprite.getTextureRect();
		if (WGTextRect.top == 472)
		{
			gameboard.watergirl.sprite.setTextureRect(sf::IntRect(currentFrame * 80 + 18, WGTextRect.top, WGTextRect.width, WGTextRect.height)); //standing

		}
		else
		{
			gameboard.watergirl.sprite.setTextureRect(sf::IntRect(currentFrame * 96 + 14, WGTextRect.top, WGTextRect.width, WGTextRect.height));//moving right
		}
		//fireboy animation
		frameWidth = 50;
		frameHeight = 100;
		sf::IntRect FBTextRect = gameboard.fireboy.sprite.getTextureRect();
		gameboard.fireboy.sprite.setTextureRect(sf::IntRect(currentFrame * 80 + 12, FBTextRect.top, FBTextRect.width, FBTextRect.height));
		//----------------------------------------------------------------- DOORS ANIMATION -------------------------------------------------
		//width 112 height 120 constant 3 slope 158 frames 18 0~17

		sf::FloatRect FBbounds = gameboard.fireboy.sprite.getGlobalBounds();
		sf::FloatRect WGbounds = gameboard.watergirl.sprite.getGlobalBounds();
		sf::FloatRect fDoor = gameboard.fDoor.getGlobalBounds();
		sf::FloatRect wDoor = gameboard.wDoor.getGlobalBounds();

		if (fDoor.intersects(FBbounds) && currentFframe < 17) // go to next frame if last frame is not reached
		{
			gameboard.fDoor.setTextureRect(sf::IntRect(3 + currentFframe * 160, 0, 112, 120));
			currentFframe++;
		}
		else if(!fDoor.intersects(FBbounds) && currentFframe > -1)
		{
			gameboard.fDoor.setTextureRect(sf::IntRect(3 + currentFframe * 160, 0, 112, 120));
			currentFframe--;
		}
		if (wDoor.intersects(WGbounds) && currentWframe < 18) // go to next frame if last frame is not reached
		{
			gameboard.wDoor.setTextureRect(sf::IntRect(3 + currentWframe * 160, 0, 112, 120));
			currentWframe++;
		}
		else if (!wDoor.intersects(WGbounds) && currentWframe > 0)
		{
			gameboard.wDoor.setTextureRect(sf::IntRect(3 + currentWframe * 160, 0, 112, 120));
			currentWframe--;
		}

		//if (isVictory)
		//{
		//	bool fFlag = 0;
		//	if (currentFrameVictory == 2)
		//		fFlag = 1;
		//	gameboard.fireboy.sprite.setTextureRect(sf::IntRect(872 + currentFrameVictory * 76,1678+ fFlag*66, 41, 72));
		//}


		//restart the animation clock
		animationClock.restart();
		currentFrame = (currentFrame + 1) % frameCount;  // Loop through frames 0 ~ 4
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

// displays time as 00:00
string Game::formattedTime(float remainingTime) const{
	int seconds = static_cast<int>(remainingTime)%60;
	int minutes = static_cast<int>(remainingTime)/60;

	stringstream MMSS;
	MMSS << setw(2) << setfill('0') << minutes << ":" << setw(2) << setfill('0') << seconds;
	return MMSS.str();
}

// updates the timer on screen
void Game::update_remainingTime()
{
	if (isVictory|| isDefeat) {
		timer_txt.setString(formattedTime(this->remainingTime));
		victorytxt[2][0].setString(formattedTime(ceil(120.f - remainingTime)));
	}
	else {

		this->remainingTime -= delTatime;    //decrement remaining time each frame
		if (this->remainingTime < 0) {
			this->remainingTime = 0;
		}
		timer_txt.setString(formattedTime(this->remainingTime));  //update the text to show current time 
	}
}




// stores in my score file txt
void Game::store_scores(int watergirl_score, int fireboy_score, float  remainingTime)  
{
	score_file.open("scores sheet.txt", ios::app);
	if (score_file.is_open()) {  
			score_file << watergirl_score << setw(10) << fireboy_score << setw(10) << remainingTime<<endl;
		score_file.close();
	}
}






