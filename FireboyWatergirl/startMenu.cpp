#include "startMenu.h"

/*
here is the implementation of the start menu functions
*/

StartMenu::StartMenu()
{
    initWin();
    initVars();
}

//initialization
void StartMenu::initVars()
{
    this->choice = 0; //sets choice to start by default
    int charSize = 30;
    //load font and backborders
    if (!this->font.loadFromFile("assets/fonts/OpenSans-Regular.ttf"))
    {
        std::cout << "ERROR:: couldn't load font\n";
    }
    this->backborders.setSize(sf::Vector2f(videoMode.width, this->videoMode.height));
    if (!this->texture.loadFromFile("assets/backgrounds/startMenu.png") )
    {
        std::cout << "ERROR:: couldn't load backborders\n";
    }
    backborders.setTexture(&this->texture);

    //Start button 
    sf::Text start;
    start.setFont(font);
    start.setString("Start");
    start.setCharacterSize(charSize);
    start.setFillColor(sf::Color::White);
    start.setOutlineColor(sf::Color::Black);
    start.setOutlineThickness(5);
    start.setPosition(400, 250);

    //rankingList button
    sf::Text rankingList;
    rankingList.setFont(font);
    rankingList.setString("Ranking List");
    rankingList.setCharacterSize(charSize);
    rankingList.setFillColor(sf::Color::White);
    rankingList.setOutlineColor(sf::Color::Black);
    rankingList.setOutlineThickness(5);
    rankingList.setPosition(400, 300);

    //exit button
    sf::Text exit;
    exit.setFont(font);
    exit.setString("Exit");
    exit.setCharacterSize(charSize);
    exit.setFillColor(sf::Color::White);
    exit.setOutlineColor(sf::Color::Black);
    exit.setOutlineThickness(5);
    exit.setPosition(400, 400);

    startMenuText[0] = start;
    startMenuText[1] = rankingList;
    startMenuText[2] = exit;

    //place every text in the center WRT screen width
    for (auto& txt : startMenuText)
        centerText(txt, *this->win);

}

void StartMenu::initWin()
{
    this->videoMode.width = 960;
    this->videoMode.height = 700;

    this->win = new sf::RenderWindow(this->videoMode, "Fireboy Watergirl", sf::Style::Default | sf::Style::Titlebar | sf::Style::Close);
    this->win->setFramerateLimit(60);
}

//Update and render
void StartMenu::update()
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
        case sf::Event::KeyReleased:
            if (ev.key.code == sf::Keyboard::Up)
                moveUp();
            else if (ev.key.code == sf::Keyboard::Down)
                moveDown();
            else if(ev.key.code == sf::Keyboard::Return) //when player presses enter: handle his choice
            { 
                int choice = startMenuPressed();
                if (choice == 0)
                {
                    //presses start: so he wants to see a window a certain level ,and game logic starts
                    Game game;
                    while (game.running())
                    {
                        //update..
                        game.update();
                        //render
                        game.render();

                    }
                }
                else if (choice == 1) //Ranking list must show descending orders of scores the logic should be implemented when constructed
                {
                    RankingList rankingList;
                    while (rankingList.running())
                    {
                        //update..
                        rankingList.update();
                        //render
                        rankingList.render();

                    }
                }
                else if (choice == 2)
                {
                    this->win->close();
                }
            }
            break;
            
        }
    }
}

void StartMenu::render()
{
    //clear window first
    this->win->clear();
    //draw 
    this->win->draw(backborders);

    for (auto& text : this->startMenuText)
        this->win->draw(text);
    //display
    this->win->display();
}

bool StartMenu::running()
{
    return this->win->isOpen();
}


//Controlls
void StartMenu::moveUp()
{
    startMenuText[choice].setStyle(sf::Text::Regular);
    choice--;
    if (choice < 0)
        choice = 2;
    startMenuText[choice].setStyle(sf::Text::Bold);

}

void StartMenu::moveDown()
{
    startMenuText[choice].setStyle(sf::Text::Regular);
    choice++;
    if (choice > 2)
        choice = 0;
    startMenuText[choice].setStyle(sf::Text::Bold);

}

int StartMenu::startMenuPressed()
{
    return this->choice;
}

StartMenu::~StartMenu() {
    delete win;
};

