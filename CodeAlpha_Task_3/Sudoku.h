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
	const int windowSize = 900;
	const int gridSize = 9;
	const int cell = windowSize / gridSize;
	sf::RenderWindow* window;
	sf::Event ev;
	sf::Font font;
	sf::Text txtGrid[9][9];
	sf::Text inputPrompt;
	sf::Text startingPrompt;
	sf::Text solvePrompt;
	sf::Text endPrompt;
	string inputText;
	sf::RectangleShape selectedCellHighlight;
	pair<int, int> selectedCell = { -1, -1 };
	vector<vector<int>> sudokuGrid;
	int row,col;
	bool started = false;
	bool selected = false;
	bool solved = false;
	bool pause = true;

	void initWindow();
	void initGrid();
	void initMenu();
	bool solveSudoku();
	bool isValid(int r, int c, int num);
	bool gridEmpty(int& r, int& c);
	void resetGrid();
	bool checkIfValid();
	void printGridToScreen();
	bool gridFilled();
public:
	Sudoku();
	virtual ~Sudoku();
	void update();
	void render();
	void pollEvent();
	const bool isRunning();	
};

