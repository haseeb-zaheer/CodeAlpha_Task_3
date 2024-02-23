#include "Sudoku.h"
#include <iostream>


void Sudoku::initWindow()
{
	this->window = new sf::RenderWindow(sf::VideoMode(windowSize, windowSize), "Sudoku Solver");
}

void Sudoku::initGrid()
{
	sudokuGrid.resize(gridSize, vector<int>(gridSize, 0));

	if (!font.loadFromFile("PTR.ttf")) {
		cout << "Error loading font!" << endl;
		exit(1);
	}

	for (int i = 0; i < gridSize; i++)
	{
		for (int j = 0; j < gridSize; j++)
		{
			txtGrid[i][j].setFont(font);
			txtGrid[i][j].setFillColor(sf::Color::Black);
			txtGrid[i][j].setOutlineColor(sf::Color::White);
			txtGrid[i][j].setOutlineThickness(1);
			txtGrid[i][j].setCharacterSize(30);
			txtGrid[i][j].setPosition(i*cell+20, j*cell+10);
		}
	}

	selectedCellHighlight.setSize(sf::Vector2f(cell, cell));
	selectedCellHighlight.setFillColor(sf::Color::Yellow);  // Light gray with some transparency

	inputPrompt.setFont(font);
	inputPrompt.setCharacterSize(30);
	inputPrompt.setFillColor(sf::Color::Black);
}

void Sudoku::render()
{
	window->clear(sf::Color::White);

	// Draw squares around each 3x3 subgrid

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

	// Draw borders around individual cells
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

	window->display();
}

Sudoku::Sudoku()
{
	this->initWindow();
	this->initGrid();
}

Sudoku::~Sudoku()
{
	delete this->window;
}

void Sudoku::update()
{
	pollEvent();
}

void Sudoku::pollEvent()
{
	inputPrompt.setPosition(selectedCell.second * cell + 20, selectedCell.first * cell + 10);
	while (this->window->pollEvent(ev))
	{
		switch (ev.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			if (ev.key.code == sf::Keyboard::Escape)
				this->window->close();
			
		case sf::Event::MouseButtonPressed:
			if (ev.key.code == sf::Mouse::Left)
			{
				inputText.clear();
				inputPrompt.setString(inputText);
				row = ev.mouseButton.y / cell;
				col = ev.mouseButton.x / cell;
				selectedCell = { row, col };

				cout << "Clicked on cell (" << row << ", " << col << ")" << std::endl;				
			}
		case sf::Event::TextEntered:
			if (ev.text.unicode >= 48 && ev.text.unicode <= 57)
				inputText += static_cast<char>(ev.text.unicode);
			else if (ev.text.unicode == 8 && !inputText.empty())
				inputText.pop_back();
			else if (ev.text.unicode == 13)
			{
				if (!inputText.empty())
				{
					int val = stoi(inputText);
					if (val >= 1 && val <= 9)
					{
						cout << ("Value set to: ", val);
						sudokuGrid[row][col] = val;
						txtGrid[row][col].setString(val == 0 ? "" : std::to_string(val));
					}
				}				
			}
			// Update the prompt text
			inputPrompt.setString(inputText);
		}
	}
}

const bool Sudoku::isRunning()
{
	return this->window->isOpen();
}
