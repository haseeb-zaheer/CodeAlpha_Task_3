#include "Sudoku.h"
#include <iostream>

Sudoku::Sudoku()
{
	this->initWindow();
	this->initGrid();
	this->initMenu();
}
Sudoku::~Sudoku()
{
	delete this->window;
}
void Sudoku::update()
{
	pollEvent();
}
void Sudoku::render()
{
	window->clear(sf::Color::White);

	if (started || solved)
	{
		for (int i = 0; i <= gridSize; i += 3) {
			for (int j = 0; j <= gridSize; j += 3) {
				sf::RectangleShape square;
				square.setSize(sf::Vector2f(cell * 3, cell * 3));
				square.setFillColor(sf::Color::Transparent);
				square.setOutlineColor(sf::Color::Black);
				square.setOutlineThickness(2);
				square.setPosition(j * cell, i * cell);

				this->window->draw(square);
			}
		}

		for (int i = 0; i < gridSize; ++i) {
			for (int j = 0; j < gridSize; ++j) {
				sf::RectangleShape cellBorder;
				cellBorder.setSize(sf::Vector2f(cell, cell));
				cellBorder.setFillColor(sf::Color::Transparent);
				cellBorder.setOutlineColor(sf::Color::Black);
				cellBorder.setOutlineThickness(1);
				cellBorder.setPosition(j * cell, i * cell);

				this->window->draw(cellBorder);
				this->window->draw(txtGrid[i][j]);
			}
		}

		if (selectedCell.first >= 0 && selectedCell.second >= 0) {
			selectedCellHighlight.setPosition(selectedCell.second * cell, selectedCell.first * cell);
			this->window->draw(selectedCellHighlight);
		}

		this->window->draw(inputPrompt);
	}
	else
	{
		this->window->draw(startingPrompt);
		this->window->draw(endPrompt);
	}

	window->display();
}
void Sudoku::pollEvent()
{
	while (this->window->pollEvent(ev))
	{
		if (started)
		{
			switch (ev.type)
			{
			case sf::Event::Closed:
				this->window->close();
				break;
			case sf::Event::KeyPressed:
				if (ev.key.code == sf::Keyboard::Escape)
					this->window->close();
				else if (ev.key.code == sf::Keyboard::Return)
				{
					if (!inputText.empty())
					{
						int val = stoi(inputText);
						if (val >= 1 && val <= 9)
						{
							sudokuGrid[row][col] = val;
							txtGrid[row][col].setString(val == 0 ? "" : to_string(val));
						}
					}
				}
				else if (ev.key.code == sf::Keyboard::S)
				{
					if (gridFilled())
					{
						pause = true;
						started = false;
						if (checkIfValid())
						{
							solved = solveSudoku();
							if (solved)
							{
								printGrid();
								printGridToScreen();
								cout << "Solved!" << endl;
							}
						}
						else
						{
							printGrid();
							cout << "Cannot be solved!" << endl;
							endPrompt.setString("Grid was not solvable!");
						}
					}
				}
				else if (ev.key.code == sf::Keyboard::R)
				{
					solved = false;
					selected = false;
					pause = false;
					started = true;
					resetGrid();
					initGrid();
					endPrompt.setString("");
				}
				break;
			case sf::Event::MouseButtonPressed:
				if (ev.mouseButton.button == sf::Mouse::Left)
				{
					selected = true;
					inputText.clear();
					inputPrompt.setString(inputText);
					row = ev.mouseButton.y / cell;
					col = ev.mouseButton.x / cell;
					selectedCell = { row, col };

					inputPrompt.setPosition(selectedCell.second * cell + 20, selectedCell.first * cell + 10);

					cout << "Clicked on cell (" << row << ", " << col << ")" << endl;
				}
				break;
			case sf::Event::TextEntered:
				if (selected)
				{
					if (ev.text.unicode >= 48 && ev.text.unicode <= 57)
						inputText += static_cast<char>(ev.text.unicode);
					else if (ev.text.unicode == 8 && !inputText.empty())
						inputText.pop_back();

					inputPrompt.setString(inputText);
				}
				break;
			}
		}
		else if (pause && solved)
		{
			switch (ev.type)
			{
			case sf::Event::Closed:
				this->window->close();
				break;
			case sf::Event::KeyPressed:
				if (ev.key.code == sf::Keyboard::R)
				{
					inputText.clear();
					inputPrompt.setString(inputText);
					solved = false;
					selected = false;
					pause = false;
					started = true;
					resetGrid();
					initGrid();
					endPrompt.setString("");
				}
				break;
			}
		}
		else if (pause)
		{
			switch (ev.type)
			{
			case sf::Event::Closed:
				this->window->close();
				break;
			case sf::Event::KeyPressed:
				if (ev.key.code == sf::Keyboard::Return)
				{
					inputText.clear();
					inputPrompt.setString(inputText);
					resetGrid();
					initGrid();
					started = true;
				}
				break;
			}
		}
	}
}
const bool Sudoku::isRunning()
{
	return this->window->isOpen();
}

void Sudoku::initWindow()
{
	this->window = new sf::RenderWindow(sf::VideoMode(windowSize, windowSize), "Sudoku Solver");
}
void Sudoku::initGrid()
{
	sudokuGrid.resize(gridSize, vector<int>(gridSize, 0));

	for (int i = 0; i < gridSize; i++)
	{
		for (int j = 0; j < gridSize; j++)
		{
			txtGrid[i][j].setString("");
			txtGrid[i][j].setFont(font);
			txtGrid[i][j].setFillColor(sf::Color::Black);
			txtGrid[i][j].setOutlineColor(sf::Color::White);
			txtGrid[i][j].setOutlineThickness(1);
			txtGrid[i][j].setCharacterSize(30);
			txtGrid[i][j].setPosition(j * cell + 20, i * cell + 10);
		}
	}

	selectedCellHighlight.setSize(sf::Vector2f(cell, cell));
	selectedCellHighlight.setFillColor(sf::Color::Yellow); 

	inputPrompt.setFont(font);
	inputPrompt.setCharacterSize(30);
	inputPrompt.setFillColor(sf::Color::Black);
}
void Sudoku::initMenu()
{
	if (!font.loadFromFile("PTR.ttf")) {
		cout << "Error loading font!" << endl;
		exit(1);
	}
	startingPrompt.setString("							Press Enter to start!\nYou can quit by pressing Escape and restart by pressing R.\n			Once you have filled the sudoku puzzle\n								Press 'S' to solve!");
	startingPrompt.setFont(font);
	startingPrompt.setCharacterSize(30);
	startingPrompt.setFillColor(sf::Color::Black);
	startingPrompt.setPosition(75, 400);

	endPrompt.setString("");
	endPrompt.setFont(font);
	endPrompt.setFillColor(sf::Color::Red);
	endPrompt.setCharacterSize(30);
	endPrompt.setPosition(sf::Vector2f(292, 330));
}
void Sudoku::printGrid()
{
	for (int i = 0; i < gridSize; i++)
	{
		for (int j = 0; j < gridSize; j++)
			cout << sudokuGrid[i][j]<<" ";
		cout << endl;
	}
}
bool Sudoku::solveSudoku()
{
	int r=0, c=0;

	if (!gridEmpty(r,c))
	{
		return true;
	}

	for (int num = 1; num <= 9; num++)
	{
		if (isValid(r, c, num))
		{
			sudokuGrid[r][c] = num;
			
			if (solveSudoku())
				return true;

			sudokuGrid[r][c] = 0;
		}
	}

	return false;
}
bool Sudoku::isValid(int r, int c, int num)
{
	for (int i = 0; i < gridSize; i++)
	{
		if (sudokuGrid[r][i] == num || sudokuGrid[i][c] == num)
			return false;
	}

	int strtRow = r - r % 3;
	int strtCol = c - c % 3;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (sudokuGrid[strtRow + i][strtCol + j] == num)
				return false;
		}
	}

	return true;
}
bool Sudoku::gridEmpty(int& r, int& c)
{
	for (r = 0; r < gridSize; r++)
		for (c = 0; c < gridSize; c++)
			if (sudokuGrid[r][c] == 0)
				return true;
	return false;
}
void Sudoku::resetGrid()
{
	for (int i = 0; i < gridSize; i++)
		for (int j = 0; j < gridSize; j++)
		{
			sudokuGrid[i][j] = 0;
			txtGrid[i][j].setString("");
			inputPrompt.setString("");
		}
}
bool Sudoku::checkIfValid()
{
	vector<int> check(this->gridSize, 0);

	for (int i = 0; i < gridSize; i++)
	{
		check.assign(gridSize, 0);
		for (int j = 0; j < gridSize; j++)
		{
			int num = sudokuGrid[i][j];
			if (num != 0 && check[num - 1] == 1)
				return false;
			else if(num!=0)
				check[num - 1] = 1;
		}
	}

	for (int i = 0; i < gridSize; i++)
	{
		check.assign(gridSize, 0);
		for (int j = 0; j < gridSize; j++)
		{
			int num = sudokuGrid[j][i];
			if (num != 0 && check[num - 1] == 1)
				return false;
			else if (num != 0)
				check[num - 1] = 1;
		}
	}

	for (int i = 0; i < gridSize; i+=3)
	{
		for (int j = 0; j < gridSize; j+=3)
		{
			check.assign(gridSize, 0);
			for (int k = 0; k < 3; k++)
			{
				for(int l =0;l<3;l++ )
				{
					int num = sudokuGrid[i + k][j + l];
					if (num != 0 && check[num - 1] == 1)
						return false;
					else if(num!=0)
						check[num - 1] = 1;
				}
			}
		}
	}

	return true;
}
void Sudoku::printGridToScreen()
{
	for (int i = 0; i < gridSize; i++)
		for (int j = 0; j < gridSize; j++)
			txtGrid[i][j].setString(to_string(sudokuGrid[i][j]));
}
bool Sudoku::gridFilled()
{
	for (int i = 0; i < gridSize; i++)
		for (int j = 0; j < gridSize; j++)
			if (sudokuGrid[i][j] != 0)
				return true;
	return false;
}