#include "Figures.h"
#include "Globals.h"
#include <iostream>

Figure::Figure(Board* bBoard, int bRow, int bColumn, Colour colour)
{
	board = bBoard;
	pos.row = bRow;
	pos.column = bColumn;
	figureColour = colour;
	textureRect = {0,0,300,400};
	firstMove = true;
	
	// if figure is white take texture from the bottom of the spritesheet
	if (colour == WHITE)
		textureRect.y += 400;
}

void Figure::draw(SDL_Renderer* renderer, TextureHandler* Hndlr)
{
	SDL_Rect destRect = {pos.column * globals::GRID_WIDTH, pos.row * globals::GRID_HEIGHT, globals::GRID_WIDTH, globals::GRID_HEIGHT};
	SDL_RenderCopy(renderer,Hndlr->load(renderer,"figures"),&textureRect, &destRect);
}

void Rook::checkMoves()
{
	forgetMoves();
	Direction dir;
	bool collision;
	Pos bufPos;
	
	for(int dirID = 0; dirID <= 7; dirID += 2) // dirID: N = 0, NE = 1, ... , NW = 7
	{
		dir = Direction(dirID);
		collision = false;
		for (int i = 1; i <= 7; i++)
		{
			bufPos = getPos();
			switch (dir)
			{
				case N:
					bufPos.row -= i;
					break;
				case E:
					bufPos.column += i;
					break;
				case S:
					bufPos.row += i;
					break;
				case W:
					bufPos.column -= i;
					break;
				default:
					break;
			}
			if (board->getFig(bufPos) != nullptr)
				collision = true;
			if(board->isValidMove(this, bufPos))
				validMoves.push_back(bufPos);
			if(collision)
				break;
			
		}
	}
}

void Rook::forgetFirstMove()
{
	if(getColour() == WHITE)
	{
		if(getPos().column == 0)
			board->eraseCastling(NW);
		else
			board->eraseCastling(NE);
	}
	else 
	{
		if(getPos().column == 0)
			board->eraseCastling(SW);
		else
			board->eraseCastling(SE);
	}
	firstMove = false;
}

void King::checkMoves()
{
	forgetMoves(); 
	Pos bufPos;
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (!(i== 0 && j==0))
			{
				bufPos = getPos();
				bufPos.row += i;
				bufPos.column += j;		
				if (board->isValidMove(this, bufPos))
					validMoves.push_back(bufPos);
			}
		}
	}
	
	Direction dir;
	int increment;
	bool validCastling;
	
	for(int dirID = 1; dirID <= 7; dirID += 2)// dirID: N = 0, NE = 1, ... , NW = 7
	{
		dir = Direction(dirID);
		if(board->getCastling(dir))
		{
			validCastling = true;
			if(dir == NE || dir == SE)
			{
				increment = 1;
				
				bufPos = getPos();
				bufPos.column = 6;
				if(board->getFig(bufPos) != 0)
					validCastling = false;
			}
			else
				increment = -1;
			
			if(dir == NE || dir == NW)
				if(figureColour != WHITE)
					validCastling = false;
			if(dir == SE || dir == SW)
				if(figureColour != BLACK)
					validCastling = false;
			
			bufPos = getPos();
			for(bufPos.column += increment; std::abs(bufPos.column - getPos().column) < 2; bufPos.column += increment)
			{
				if(!(board->isValidMove(this,bufPos)))
					validCastling = false;
			}
			
			if (validCastling)
			{
				bufPos = getPos();
				bufPos.column += 2*increment;
				validMoves.push_back(bufPos);
			}
			
		}
	}
}

void King::forgetFirstMove()
{
	if(getColour() == WHITE)
	{
		board->eraseCastling(NW);
		board->eraseCastling(NE);
	}
	else 
	{
		board->eraseCastling(SW);
		board->eraseCastling(SE);
	}
	firstMove = false;
}
	
void Queen::checkMoves()
{
	forgetMoves();
	Pos bufPos;
	Direction dir;
	bool collision;
	
	for(int dirID = 0; dirID <= 7; dirID ++)
	{
		dir = Direction(dirID);
		collision = false;
		for (int i = 1; i <= 7; i++)
		{
			bufPos = getPos();
			switch (dir)
			{
				case N:
				{
					bufPos.row -= i;						
					break;
				}
				case NE:
				{
					bufPos.row -= i;
					bufPos.column += i;
					break;
				}					
				case E:
				{
					bufPos.column += i;
					break;
				}
				case SE:
				{
					bufPos.row += i;
					bufPos.column += i;
					break;
				}
				case S:
				{
					bufPos.row += i;
					break;
				}	
				case SW:
				{
					bufPos.row += i;
					bufPos.column -= i;
					break;
				}
				case W:
				{
					bufPos.column -= i;	
					break;
				}
				case NW:
				{
					bufPos.row -= i;
					bufPos.column -= i;
					break;
				}
				default:
					break;
			}
			if (board->getFig(bufPos) != nullptr)
				collision = true;
			if(board->isValidMove(this, bufPos))
				validMoves.push_back(bufPos);
			if (collision)
				break;
		}
	}
}

void Pawn::checkMoves()
{
	forgetMoves();
	Pos bufPos = getPos();
	int increment;
	bool collision = false;
	
	if(getColour() == WHITE)
		increment = 1;
	else
		increment = -1;
	
	bufPos.row += increment;
	
	if(board->getFig(bufPos) == nullptr)
	{
		if(board->isValidMove(this, bufPos))
			validMoves.push_back(bufPos);
	}
	else
		collision = true;
	
	//check if we can eat someone
	bufPos.column = getPos().column + 1;
	if(board->isValidMove(this, bufPos) && board->getFig(bufPos) != nullptr)
		validMoves.push_back(bufPos);
	bufPos.column = getPos().column - 1;
	if(board->isValidMove(this, bufPos) && board->getFig(bufPos) != nullptr)
		validMoves.push_back(bufPos);	
		
	//
	bufPos.column = getPos().column;
	if(isFirstMove() && !collision)
	{
		bufPos.row += increment;
		if(board->isValidMove(this, bufPos) && board->getFig(bufPos) == nullptr)
			validMoves.push_back(bufPos);
	}
}

void Bishop::checkMoves()
{
	forgetMoves();
	Pos bufPos;
	Direction dir;
	bool collision;
	
	for(int dirID = 1; dirID <= 7; dirID += 2)
	{
		dir = Direction(dirID);			
		collision = false;
		for(int i = 1; i <= 7; i++)
		{
			bufPos = getPos();
			switch (dir)
			{
				case NE:
					bufPos.row -= i;
					bufPos.column += i;
					break;
				case SE:
					bufPos.row += i;
					bufPos.column += i;
					break;	
				case SW:
					bufPos.row += i;
					bufPos.column -= i;
					break;
				case NW:
					bufPos.row -= i;
					bufPos.column -= i;
					break;
				default:
					break;
			}
			if (board->getFig(bufPos) != nullptr)
				collision = true;
			if(board->isValidMove(this, bufPos))
				validMoves.push_back(bufPos);
			if (collision)
				break;
		}
	}	
}

void Knight::checkMoves()
{
	forgetMoves();
	Direction dir;
	Pos bufPos;
	
	for(int dirID = 0; dirID <= 7; dirID += 2)
	{
		dir = Direction(dirID);
		for(int i = -1; i <= 1; i +=2)
		{
			bufPos = getPos();
			switch (dir)
			{
				case N:
					bufPos.row -= 2;
					bufPos.column += i;
					break;
				case E:
					bufPos.row += i;
					bufPos.column += 2;
					break;
				case S:
					bufPos.row += 2;
					bufPos.column += i;
					break;
				case W:
					bufPos.row += i;
					bufPos.column -= 2;
					break;
				default:
					break;
			}
			if(board->isValidMove(this, bufPos))
				validMoves.push_back(bufPos);
				
		}
	}
}