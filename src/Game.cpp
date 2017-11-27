#include <iostream>

#include "Game.h"

#include "globals.h"

Game::Game(){
}

Game::Game(std::string windowName, int width, int height)
{
	window = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	if (window != nullptr)
	{
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer != nullptr)
		{
			gameRunning = true;
		}
	}
}

Game::~Game(){
}

void Game::gameLoop()
{
	init();
	while(gameRunning)
	{
		eventHandler();
		render();
	}
}

void Game::init()
{
	board.init();
	gameRunning = true;
}
	

void Game::eventHandler()
{
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0 )
	{
		int mouseX,mouseY,row,column;
		Pos pos;
		switch (e.type)
		{
			case SDL_QUIT :
				gameRunning = false;
				break;
			
			case SDL_MOUSEBUTTONDOWN:
				mouseX = e.motion.x;
				column = mouseX / globals::GRID_WIDTH;
				mouseY = e.motion.y;
				row = mouseY / globals::GRID_HEIGHT;
				pos = {row,column};
				board.click(pos);
				break;
			
			default:
				break;
		}
	}		
}

void Game::render(){
	
	SDL_RenderClear(renderer);
	board.draw(getRen(),getHndlr());
	SDL_RenderPresent(renderer);
}