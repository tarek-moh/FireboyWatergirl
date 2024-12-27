#include "rankingList.h"
using namespace std;

RankingList::RankingList()
{
	initVars();
	initWin();
}

void RankingList::initVars()
{
	//read file
	read_file();
	bubble_sorting(this->vec);

	//initialize font 
	if (!winbackgrT.loadFromFile("assets/backgrounds/startMenu.PNG")){
		cout << "couldn't load  background!!\n";
	}
	winbackgr.setTexture(winbackgrT);
	winbackgr.setScale(1.6, 1.6);

	//headline (rank list) setup
	if (!headlineT.loadFromFile("assets/images/ranklist text.PNG")) {
		cout << "couldn't load rank list headline !!\n";
	}
	headline.setTexture(headlineT);
	headline.setScale(0.6,0.6);
	headline.setPosition(300, 72);

	if (!font.loadFromFile("assets/fonts/OpenSans-Regular.ttf")) {
		cout << "couldn't load font for headline !!\n";
	}
	headlineTxt.setFont(font);
	headlineTxt.setString("Rank List");
	headlineTxt.setCharacterSize(50);
	headlineTxt.setFillColor(sf::Color::Yellow);
	headlineTxt.setPosition(370,70);
	//headlineTxt.setOutlineColor(sf::Color::Yellow);
	//headlineTxt.setOutlineThickness(2);


	//list  setup
	if (!listT.loadFromFile("assets/images/ranklist_backg.PNG")) {
		cout << "couldn't load rank list background !!\n";
	}
	list.setTexture(listT);
	list.setScale(0.93,1.18);
	
    
	// headlines setup
	for (int i = 0; i < 4; i++) {
		headers_txt[i].setFont(font);
		headers_txt[i].setFillColor(sf::Color::Yellow);
	}
	headers_txt[0].setPosition(80, 140);
	headers_txt[1].setPosition(230, 140);
	headers_txt[2].setPosition(480, 140);
	headers_txt[3].setPosition(700, 140);

	headers_txt[0].setString("Rank");
	headers_txt[1].setString("Blue Gems");
	headers_txt[2].setString("Red Gems");
	headers_txt[3].setString("Elapsed Time");

	//rank details
	//while (vec.size() <= 10) {
		for (int i = 0; i < vec.size(); i++) {

			for (int j = 0; j < 4; j++) {
				details_txt[i][j].setFont(font);
				details_txt[i][j].setFillColor(sf::Color::Yellow);
				details_txt[i][j].setCharacterSize(25);

			}

			details_txt[i][0].setString(to_string(i + 1));
			details_txt[i][1].setString(to_string(get<0>(this->vec[i])));
			details_txt[i][2].setString(to_string(get<1>(this->vec[i])));
			details_txt[i][3].setString(formattedTime(120 - get<2>(this->vec[i])));

			// setting texts pos
			details_txt[i][0].setPosition(100, 200 + 40 * i);
			details_txt[i][1].setPosition(300, 200 + 40 * i);
			details_txt[i][2].setPosition(540, 200 + 40 * i);
			details_txt[i][3].setPosition(760, 200 + 40 * i);
		}
	//}
}

void RankingList::initWin() {
	this->videoMode.width = 960;
	this->videoMode.height = 700;
	this->win = new sf::RenderWindow(this->videoMode, "Ranking List", sf::Style::Default | sf::Style::Titlebar | sf::Style::Close);
	this->win->setFramerateLimit(60);
}

//Window related functions handles any events and renders them in the window
void RankingList::render()
{
	//clear window
	this->win->clear();

	//draw the list
	this->win->draw(list);
	this->win->draw(headline);
	this->win->draw(headlineTxt);

	for (int i = 0; i < 4; i++) {
		this->win->draw(headers_txt[i]);
	}

	for (int i = 0; i < vec.size(); i++) {
		for (int j = 0; j < 4; j++) {
			this->win->draw(details_txt[i][j]);
		}
	}

	//display
	this->win->display();
}

void RankingList::update() //needs implemention !!!!  (I KNOWWW!!!!)
{
		while (this->win->pollEvent(this->ev)) {
			switch (this->ev.type){
			case sf::Event::Closed:
				this->win->close();
				break;
			}
		}
	
}

bool RankingList::running()
{
	return this->win->isOpen();
}

RankingList::~RankingList()
{
	delete win;
}

//calculate the sorting criterion
float RankingList::evaluated_weight(int x, int y, float z)
{
	return ((x+y)*z);
}

//sort stored data in descending order 
void RankingList::bubble_sorting(vector<tuple<int,int, float>>&data)
{
	for (int i = 0; i < data.size()-1; i++) {
		for (int j = 0; j < data.size() - 1; j++) {
	         
			float row1 = evaluated_weight(get<0>(data[j]), get<1>(data[j]), get<2>(data[j]));
			float row2 = evaluated_weight(get<0>(data[j + 1]), get<1>(data[j + 1]), get<2>(data[j + 1]));
			if (row1 < row2)
			{
				swap(get<0>(data[j]), get<0>(data[j + 1]));
				swap(get<1>(data[j]), get<1>(data[j + 1]));
				swap(get<2>(data[j]), get<2>(data[j + 1]));
			}
		}
	}
}

//read data from file
void RankingList::read_file()
{
	this->file.open("scores sheet.txt", ios::in); //open file for reading
	if (!file.is_open()) {
		cout << "couldn't load txt file!!";  //check if file is open 
	}
	while (file >> this->watergirl_score >>this-> fireboy_score >> this->remainingTime) {
		this->vec.push_back(make_tuple(this->watergirl_score, this->fireboy_score, this->remainingTime)); //read from file
	}
	this->file.close();  //close file after reading
	
}

// displays time as 00:00
string RankingList::formattedTime(float time) const
{
	int seconds = static_cast<int>(time) % 60;
	int minutes = static_cast<int>(time) / 60;

	stringstream MMSS;
	MMSS << setw(2) << setfill('0') << minutes << ":" << setw(2) << setfill('0') << seconds;
	return MMSS.str();
}



