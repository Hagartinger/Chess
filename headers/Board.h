#ifndef BOARD_H
#define BOARD_H

#include <vector>

#include "TextureHandler.h"
#include "Globals.h"

class Figure;

class Board
{
public:
	Board();
	~Board();
	
	//place figures on their pregame position
	void init();
	
	//creates figures and remembers it's position
	void placeFigure(FigureType figType, int row, int column, Colour colour);
	
	//draws board and figures on the screen
	void draw(SDL_Renderer* renderer, TextureHandler* Hndlr);
	
	//Logic for click event
	void click(Pos pos);
	
	//moves selectedFigure to this position
	void makeMove(Pos pos);
	
	//moves Rook in the right place
	void makeCastling(Direction rookPos);
	
	//goes threw figuresOnBoard and calls checkMoves of each figure of given colour
	void checkMoves(Colour colour);
	
	// returns true if it's posible to move this figure to current position
	bool isValidMove(Figure* fig, Pos pos);
	
	//returns pointer to a figure on this position
	Figure* getFig(Pos pos);
	
	//sets check-trigger of current colour to true
	void checkDetected(Colour colour);
	
	//returns true if player of this colour is checkmated
	bool checkMateDetection(Colour colour);
	
	
	
	//sets current castling-trigger to false
	void eraseCastling(Direction rookPos);
	
	//returns castling-trigger
	bool getCastling(Direction rookPos);
	
private:
	Figure* figuresOnBoard[8][8];
	Colour curPlayer;
	Figure* selectedFigure;
	std::vector<Pos>* alowedMoves;
	
	bool whiteChecked, blackChecked, checkInProgress;
	bool whiteLeftCastling, whiteRightCastling, blackLeftCastling, blackRightCastling;
	
	
	
};

#endif

