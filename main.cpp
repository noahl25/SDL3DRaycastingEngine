#include <iostream>
#include <SDL.h>

#include "Game.h"

int main(int argc, char** args) {
 
	Game game(1000, 500);
	game.Run();
	return 0;
}