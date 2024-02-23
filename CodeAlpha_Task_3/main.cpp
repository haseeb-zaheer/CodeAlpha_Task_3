#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include "Sudoku.h"

using namespace std;
int main()
{
	Sudoku inst;
	
	while (inst.isRunning())
	{
		inst.update();
		inst.render();
	}

	return 0;
}