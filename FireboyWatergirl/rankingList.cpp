#include "rankingList.h"

RankingList::RankingList()
{
	initVars();
	initWin();
}

void RankingList::initVars()
{
}

void RankingList::initWin() {
	this->videoMode.width = 960;
	this->videoMode.height = 700;

	this->win = new sf::RenderWindow(this->videoMode, "Fireboy Watergirl", sf::Style::Default | sf::Style::Titlebar | sf::Style::Close);
	this->win->setFramerateLimit(60);
}

//Window related functions handles any events and renders them in the window
void RankingList::render()
{
	//clear window
	this->win->clear();
	//draw the list
	//
	//.....
	//
	//display
	this->win->display();
}

void RankingList::update() //needs implemention !!!!
{

}

bool RankingList::running()
{
	return this->win->isOpen();
}

RankingList::~RankingList()
{
}
