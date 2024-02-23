#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <vector>
using namespace std;

class Sudoku
{
private:
	const int windowSize = 1000;
	const int gridSize = 9;
	const int cell = windowSize / gridSize;
	sf::RenderWindow* window;
	sf::Event ev;
	sf::Font font;
	sf::Text txtGrid[9][9];
	sf::Text inputPrompt;
	string inputText;
	sf::RectangleShape selectedCellHighlight;
	pair<int, int> selectedCell = { -1, -1 };
	vector<vector<int>> sudokuGrid;
	int row,col;

	void initWindow();
	void initGrid();
public:
	Sudoku();
	virtual ~Sudoku();
	void update();
	void render();
	void pollEvent();
	const bool isRunning();	
};

