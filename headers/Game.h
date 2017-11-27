#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
#include <SDL.h>

#include "TextureHandler.h"
#include "Board.h"

class Game{
public:
	Game();
	Game(std::string windowName, int width, int height);
	~Game();
	
	//main loop
	void gameLoop();
	
	//Renders everything on the screen
	void render();

	
	// Handles events (input)
	void eventHandler();
	
	//Prepare the desk
	void init();
	
	SDL_Renderer* getRen(){return renderer;}
	TextureHandler* getHndlr(){return &textHndlr;}

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	
	bool gameRunning;
	TextureHandler textHndlr;
	
	Board board;

};
	
#endif