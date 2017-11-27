#ifndef FIGURES_H
#define FIGURES_H

#include <SDL.h>
#include <vector>

#include "Board.h"
#include "TextureHandler.h"
#include "Globals.h"

class Figure{
public:
	Figure(){};
	Figure(Board* bBoard, int bRow, int bColumn, Colour colour);
	
	// Draws figure on the screen
	void draw(SDL_Renderer* renderer, TextureHandler* Hndlr);
	
	// fills vector of valid moves for this figure
	virtual void checkMoves() = 0;
	
	//returns pointer for vector of valid moves
	std::vector<Pos>* getValidMoves(){return &validMoves;}
	
	//erases vector of valid moves
	void forgetMoves()
	{
		while(validMoves.size() != 0)
			validMoves.pop_back();
	}
	
	Colour getColour() { return figureColour;}
	FigureType getType() { return figureType;}
	Pos getPos(){ return pos;}
	bool isFirstMove(){return firstMove;}
	
	virtual void forgetFirstMove(){firstMove = false;}

	void setPos(Pos gpos){ pos = gpos;}
protected:
	Pos pos;
	SDL_Rect textureRect;
	FigureType figureType;
	Colour figureColour;
	bool firstMove;
	Board* board;
	std::vector<Pos> validMoves;
};

class Rook: public Figure{
public:
	Rook(){}
	Rook(Board* bBoard, int bRow, int bColumn, Colour colour):Figure(bBoard,bRow,bColumn,colour)
	{
		// First figure from spritesheet
		textureRect.x += 0 * 300;
		figureType = ROOK;
	}	
	void forgetFirstMove();
	void checkMoves();

};

class Bishop : public Figure{
public:
	Bishop(){};
	Bishop(Board* bBoard, int bRow, int bColumn, Colour colour):Figure(bBoard,bRow,bColumn,colour)
	{
		// Second figure from spritesheet
		textureRect.x += 1 * 300;
		figureType = BISHOP;
	}	
	void checkMoves();
};


class Queen : public Figure{
public:
	Queen(){};
	Queen(Board* bBoard,int bRow, int bColumn, Colour colour):Figure(bBoard,bRow,bColumn,colour)
	{
		// Third figure from spritesheet
		textureRect.x += 2 * 300;
		figureType = QUEEN;
	}	
	void checkMoves();
};

class King : public Figure{
public:
	King(){};
	King(Board* bBoard, int bRow, int bColumn, Colour colour):Figure(bBoard,bRow,bColumn,colour)
	{
		// Fourh figure from spritesheet
		textureRect.x += 3 * 300;
		figureType = KING;
	}	
	void forgetFirstMove();
	void checkMoves();
};

class Knight : public Figure{
public:
	Knight(){};
	Knight(Board* bBoard, int bRow, int bColumn, Colour colour):Figure(bBoard,bRow,bColumn,colour)
	{
		// Fifth figure from spritesheet
		textureRect.x += 4 * 300;
		figureType = KNIGHT;
	}	
	void checkMoves();
};

class Pawn : public Figure{
public:
	Pawn(){};
	Pawn(Board* bBoard, int bRow, int bColumn, Colour colour):Figure(bBoard,bRow,bColumn,colour)
	{
		// Sixth figure from spritesheet
		textureRect.x += 5 * 300;
		figureType = PAWN;
	}	
	void checkMoves();

};
#endif