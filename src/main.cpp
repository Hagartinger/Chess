#include "SDL.h"

#include "Game.h"
#include "globals.h"

int main(int argc, char* argv[])
{
	Game chess = Game("Chess", globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT);
	
	chess.gameLoop();
	
	return 0;
}