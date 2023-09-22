#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#define SCREEN_RESOLUTION_HORIZONTAL	700
#define SCREEN_RESOLUTION_VERTICAL	530

#define GAME_FIELD_SIZE_X	30
#define GAME_FIELD_SIZE_Y	30

#define GAME_STEIN_SIZE_X	22.3
#define GAME_STEIN_SIZE_Y	10.6

#define GAME_INFOBAR_SIZE	30


using namespace std;

enum steinType
{
	BLACK,
	WHITE,
	RED,
	GREEN,
	BLUE,
	YELLOW,
	MAGENTA,
	CYAN
};

class stein
{
	private:
	public:
	steinType type;
	bool inGame;
	
	stein()
	{
		inGame = true;
	}
	
	~stein()
	{
	}
};

class gameField
{
	private:
	public:
	float element_size_x;
	float element_size_y;
	int field_x;
	int field_y;
	stein **field;
	int score;
	int counter;
	
	gameField(int _field_x, int _field_y, float _element_size_x, float _element_size_y)
	{
		field = new stein*[_field_x];
		if (!field)
		{
			cerr << "memory alloc err" << endl;
			exit(-10);
		}
		for (int i = 0; i < _field_x; i++)
		{
			field[i] = new stein[_field_y];
			if (!field[i])
			{
				cerr << "memory alloc err" << endl;
				exit(-10);
			}
		}
		field_x = _field_x;
		field_y = _field_y;
		element_size_x = _element_size_x;
		element_size_y = _element_size_y;
		score = 0;
		counter = _field_x * _field_y;
	}
	~gameField()
	{
		for (int i = 0; i < field_y; i++)
		{
			delete [] field[i];
		}
		delete [] field;
	}
	
	void create()
	{
		srand(time(0));
		for (int i = 0; i < field_x; i++)
		{
			for (int j = 0; j < field_y; j++)
			{
				switch (rand() % 7)
				{
					case 0:
						field[i][j].type = WHITE;
						break;
					case 1:
						field[i][j].type = RED;
						break;
					case 2:
						field[i][j].type = GREEN;
						break;
					case 3:
						field[i][j].type = BLUE;
						break;
					case 4:
						field[i][j].type = YELLOW;
						break;
					case 5:
						field[i][j].type = MAGENTA;
						break;
					case 6:
						field[i][j].type = CYAN;
						break;
				}
			}
		}
	}
	
	void bang(int x, int y, int type)
	{
		if (field[x][y].inGame)
		{
			field[x][y].inGame = false;
			field[x][y].type = BLACK;
			if (x > 0)
				if (field[x-1][y].type == type)
					bang(x - 1, y, type);
			if (x < field_x - 1)
				if (field[x+1][y].type == type)
					bang(x + 1, y, type);
			if (y > 0)
				if (field[x][y-1].type == type)
					bang(x, y - 1, type);
			if (y < field_y - 1)
				if (field[x][y+1].type == type)
					bang(x, y + 1, type);
			score++;
			counter--;
		}
	}
	
	void fallen(int x, int y)
	{
		stein temp;
		if (!field[x][y].type)
		{
			temp = field[x][y];
			field[x][y] = field[x][y-1];
			field[x][y-1] = temp;
		}
	}
	
	void alleFallen()
	{
		for (int j = field_y - 1; j > 0; j--)
		{
			for (int i = 0; i < field_x; i++)
			{
				fallen(i, j);
			}
		}
	}
};

int main()
{
	double screen_resolution_x = GAME_FIELD_SIZE_X * GAME_STEIN_SIZE_X;
	double screen_resolution_y = GAME_FIELD_SIZE_Y * GAME_STEIN_SIZE_Y +
								GAME_INFOBAR_SIZE;
	
	//double element_size_x = screen_resolution_x / GAME_FIELD_SIZE;
	
	gameField pole(GAME_FIELD_SIZE_X, GAME_FIELD_SIZE_Y,
					GAME_STEIN_SIZE_X, GAME_STEIN_SIZE_Y);
	int score = 0;
	int score_tmp;
	
	pole.create();

	sf::RenderWindow window(sf::VideoMode(screen_resolution_x, screen_resolution_y), "Shitty");
	sf::Event event;
	sf::RectangleShape rec(sf::Vector2f(pole.element_size_x, pole.element_size_y));
	sf::Vector2i mousePosition;
	
	int mouseOnField_x;
	int mouseOnField_y;
	
	/////////////////////////////////////////////////////////////////
	///
	///
	///
	//Прямоугольник для панели информации
    sf::RectangleShape bar(sf::Vector2f(screen_resolution_x, GAME_INFOBAR_SIZE));
    bar.setFillColor(sf::Color::Black);
    bar.setPosition(0, screen_resolution_y - GAME_INFOBAR_SIZE);

    //Далее текстовой информации
    //Загрузка шрифта
    sf::Font font;
    font.loadFromFile("resources/consola.ttf");

	//Информация о количестве набранных очков
    sf::Text score_text("", font, 20);
    score_text.setColor(sf::Color::White);
    score_text.setPosition(30, screen_resolution_y - GAME_INFOBAR_SIZE);
    char sc[50]; //сюда будет помещён сам текст

    //Надпись "Game Over"
    sf::Text endspiel("You are shitty", font, 50);
    endspiel.setColor(sf::Color::Red);
    endspiel.setStyle(sf::Text::Bold);
    endspiel.setPosition(50, 200);
    
    //Надпись "WIN"
    sf::Text winner("Complete!", font, 50);
    winner.setColor(sf::Color::Red);
    winner.setStyle(sf::Text::Bold);
	winner.setPosition(70, 200);
	/////////////////////////////////////////////////////////////////
	while (window.isOpen())
	{
		mousePosition = sf::Mouse::getPosition(window);
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
				
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if ((event.mouseButton.button == sf::Mouse::Left) &&
					(mousePosition.x <= screen_resolution_x) &&
					(mousePosition.y <= screen_resolution_y))
				{
					mouseOnField_x = mousePosition.x / pole.element_size_x;
					mouseOnField_y = mousePosition.y / pole.element_size_y;
					score_tmp = pole.score;
					pole.bang(mouseOnField_x, mouseOnField_y, pole.field[mouseOnField_x][mouseOnField_y].type);
					if (pole.score - score_tmp == 1) score--;
					if (pole.score - score_tmp == 3) score++;
					if (pole.score - score_tmp > 3) score = score + (pole.score - score_tmp);
					
					cout << pole.counter << endl;
					
					
					
				}
			}
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			window.close();
		}
		
		window.clear();
		
		
					
		for (int i = 0; i < pole.field_x; i ++)
		{
			for (int j = 0; j < pole.field_y; j++)
			{
				switch (pole.field[i][j].type)
				{
					case BLACK:
						rec.setFillColor(sf::Color::Black);
						break;
					case WHITE:
						rec.setFillColor(sf::Color::White);
						break;
					case RED:
						rec.setFillColor(sf::Color::Red);
						break;
					case GREEN:
						rec.setFillColor(sf::Color::Green);
						break;
					case BLUE:
						rec.setFillColor(sf::Color::Blue);
						break;
					case YELLOW:
						rec.setFillColor(sf::Color::Yellow);
						break;
					case MAGENTA:
						rec.setFillColor(sf::Color::Magenta);
						break;
					case CYAN:
						rec.setFillColor(sf::Color::Cyan);
						break;
				}
				rec.setPosition(sf::Vector2f(i * pole.element_size_x, j * pole.element_size_y));
				window.draw(rec);
				pole.alleFallen();
			}
		}
		
		if (pole.counter <= 0)
		{
			if (score > 0) window.draw(winner);
			else window.draw(endspiel);
		}
		sprintf(sc, "Score: %d ", score);
        score_text.setString(sc);
		window.draw(bar);
		window.draw(score_text);
				
		window.display();
	}
	return 0;
}
