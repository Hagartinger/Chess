#include <SDL.h>
#include <cmath>

#include "Board.h"
#include "Globals.h"
#include "Figures.h"
#include <iostream>

Board::Board()
{
	for(int i = 0; i <= 7; i++)
	{
		for(int j = 0; j <= 7; j++)
		{
			figuresOnBoard[i][j] = nullptr;
		}
	}
	curPlayer = WHITE;
	selectedFigure = nullptr;
	whiteChecked = false;
	blackChecked = false;
	checkInProgress = false;
}

Board::~Board()
{
	for(int i = 0; i <= 7; i++)
	{
		for(int j = 0; j <= 7; j++)
		{
			if(figuresOnBoard[i][j] != nullptr)
			{
				delete(figuresOnBoard[i][j]);
				figuresOnBoard[i][j] = nullptr;
			}
		}
	}
}

void Board::init()
{
	// White side
	Colour colour = WHITE;
	
	placeFigure(ROOK, 0, 0, colour);
	placeFigure(ROOK, 0, 7, colour);
	placeFigure(KNIGHT, 0, 1, colour);
	placeFigure(KNIGHT, 0, 6, colour);
	placeFigure(BISHOP, 0, 2, colour);
	placeFigure(BISHOP, 0, 5, colour);
	placeFigure(QUEEN, 0, 4, colour);
	placeFigure(KING, 0, 3, colour);
	for (int i = 0; i <= 7; i++)
	{
		placeFigure(PAWN, 1, i, colour);
	}
	
	// Black side
	colour = BLACK;
	
	placeFigure(ROOK, 7, 0, colour);
	placeFigure(ROOK, 7, 7, colour);
	placeFigure(KNIGHT, 7, 1, colour);
	placeFigure(KNIGHT, 7, 6, colour);
	placeFigure(BISHOP, 7, 2, colour);
	placeFigure(BISHOP, 7, 5, colour);
	placeFigure(QUEEN, 7, 4, colour);
	placeFigure(KING, 7, 3, colour);
	for (int i = 0; i <= 7; i++)
	{
		placeFigure(PAWN, 6, i, colour);
	}
	
	checkMoves(WHITE);

	whiteRightCastling = true;
	whiteLeftCastling = true;
	blackRightCastling = true;
	blackLeftCastling = true;
}


bool Board::isValidMove(Figure* fig, Pos pos)
{
	if(pos.row < 0 || pos.row > 7 || pos.column < 0 || pos.column > 7)
		return false;
	
	if (figuresOnBoard[pos.row][pos.column] != nullptr)
	{
		if(fig->getColour() == figuresOnBoard[pos.row][pos.column]->getColour())
			return false;
		if(figuresOnBoard[pos.row][pos.column]->getType() == KING )
		{
			checkDetected(figuresOnBoard[pos.row][pos.column]->getColour());
			return false;
		}
	}

	if(!checkInProgress)
	{
		bool remWhiteChecked = whiteChecked;
		bool remBlackChecked = blackChecked;
		bool result = true;
		checkInProgress = true;
		Pos remPos = fig->getPos();
		Figure* bufFigure = figuresOnBoard[pos.row][pos.column];
		
		if(whiteChecked)
			whiteChecked = false;
		if(blackChecked)
			blackChecked = false;
		
		//makes pseudo move
		figuresOnBoard[pos.row][pos.column] = fig;
		figuresOnBoard[remPos.row][remPos.column] = nullptr;
		fig->setPos(pos);
		
		if(fig->getColour() == WHITE)
			checkMoves(BLACK); //calls all black checkmoves to see if it will call "checkDetected" function
		else
			checkMoves(WHITE);
		
		//undo pseudo move
		figuresOnBoard[remPos.row][remPos.column] = figuresOnBoard[pos.row][pos.column];
		figuresOnBoard[pos.row][pos.column] = bufFigure;
		fig->setPos(remPos);
			
		checkInProgress = false;
		
		if(whiteChecked || blackChecked)
			result = false;
		
		whiteChecked = remWhiteChecked;
		blackChecked = remBlackChecked;
		
		if(!result)
			return false;
	}
	
	return true;
}

void Board::placeFigure(FigureType figType, int row, int column, Colour colour)
{
	switch(figType)
	{
	case PAWN:
		figuresOnBoard[row][column] = new Pawn(this, row, column, colour);
		break;
	case QUEEN:
		figuresOnBoard[row][column] = new Queen(this, row, column, colour);
		break;
	case KING:
		figuresOnBoard[row][column] = new King(this, row, column, colour);
		break;
	case ROOK:
		figuresOnBoard[row][column] = new Rook(this, row, column, colour);
		break;
	case KNIGHT:
		figuresOnBoard[row][column] = new Knight(this, row, column, colour);
		break;
	case BISHOP:
		figuresOnBoard[row][column] = new Bishop(this, row, column, colour);
		break;
	default:
		break;
	}
}

void Board::draw(SDL_Renderer* renderer, TextureHandler* Hndlr)
{
	//Drawing a board and figures
	SDL_RenderCopy(renderer, Hndlr->load(renderer,"Chess_Board"),0,0);
	for (int i = 0; i <= 7; i++)
	{
		for (int j =0; j <= 7; j++)
		{
			if(figuresOnBoard[i][j] != nullptr)
				figuresOnBoard[i][j]->draw(renderer,Hndlr);
		}
	}
	
	//drawing visual boxes for selected figure and allowed moves for it
	if (selectedFigure != nullptr)
	{
		//Green rectangle for selected figure
		SDL_Rect bufRect = {0,0,globals::GRID_WIDTH, globals::GRID_HEIGHT};
		bufRect.x = selectedFigure->getPos().column * globals::GRID_WIDTH;
		bufRect.y = selectedFigure->getPos().row * globals::GRID_HEIGHT;
		SDL_SetRenderDrawColor(renderer, 0,255,100,150);
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_RenderFillRect(renderer, &bufRect);
		
		//Blue rectangles for allowed moves
		SDL_SetRenderDrawColor(renderer, 0,100,255,150);
		if(alowedMoves != nullptr)
			for (int i = 0; i < alowedMoves->size(); i++)
			{
				bufRect.x = alowedMoves->at(i).column * globals::GRID_WIDTH;
				bufRect.y = alowedMoves->at(i).row * globals::GRID_HEIGHT;
			
				SDL_RenderFillRect(renderer, &bufRect);
			}
	}
}

void Board::click(Pos pos)
{
	if(selectedFigure == nullptr)
	{//figure hasn't been selected yet
		if(figuresOnBoard[pos.row][pos.column] == 0)
			return;
		if(figuresOnBoard[pos.row][pos.column]->getColour() != curPlayer)
			return;
		selectedFigure = figuresOnBoard[pos.row][pos.column];
		alowedMoves = selectedFigure->getValidMoves();
	}	
	else
	{//figure has been selected
		if (pos.row == selectedFigure->getPos().row && pos.column == selectedFigure->getPos().column)
		{//clicked on selected figure
			selectedFigure = nullptr;
			alowedMoves = nullptr;
			return;
		}
		
		for(int i = 0; i < alowedMoves->size(); i++)
		{
			if( pos.row == alowedMoves->at(i).row && pos.column == alowedMoves->at(i).column)
			{
				makeMove(pos);
				if(curPlayer == WHITE)
				{
					curPlayer = BLACK;
					checkMoves(BLACK);
					if(checkMateDetection(BLACK))
					{
						std::cout<<"White Player WINS!"<<std::endl;
					}
				}
				else
				{
					curPlayer = WHITE;
					checkMoves(WHITE);
					if(checkMateDetection(WHITE))
					{
						std::cout<<"White Player WINS!"<<std::endl;
					}
				}
				break;
			}
		}
	}
}

void Board::makeMove(Pos pos)
{
	bool castling = false;
	Direction castlingDir;
	
	if(selectedFigure->isFirstMove())
	{
		if(selectedFigure->getType() == KING && std::abs(selectedFigure->getPos().column - pos.column) > 1)
		{//it's castling move
			castling = true;
			if(selectedFigure->getColour() == WHITE)
			{
				if(selectedFigure->getPos().column - pos.column > 0)
					castlingDir = NW;
				else
					castlingDir = NE;
			}
			else
			{
				if(selectedFigure->getPos().column - pos.column > 0)
					castlingDir = SW;
				else
					castlingDir = SE;
			}
		}
		selectedFigure->forgetFirstMove();
	}
	
	figuresOnBoard[selectedFigure->getPos().row][selectedFigure->getPos().column] = 0;
	delete(figuresOnBoard[pos.row][pos.column]);
	figuresOnBoard[pos.row][pos.column] = selectedFigure;
	selectedFigure->setPos(pos);
	
	if(castling)
		makeCastling(castlingDir);//place rook for castling move
	
	if(whiteChecked)
		whiteChecked = false;
	if(blackChecked)
		blackChecked = false;
	
	checkMoves(curPlayer); //in case new combination of figures triggers a check for opposite player
	
	selectedFigure = nullptr;
	alowedMoves = nullptr;
}

void Board::makeCastling(Direction rookPos)
{
	Pos movePosition;
	switch(rookPos)
	{
		case NE:
			selectedFigure = figuresOnBoard[0][7];
			movePosition.row = 0;
			movePosition.column = 4;
			makeMove(movePosition);
			break;
		case NW:
			selectedFigure = figuresOnBoard[0][0];
			movePosition.row = 0;
			movePosition.column = 2;
			makeMove(movePosition);
			break;
		case SE:
			selectedFigure = figuresOnBoard[7][7];
			movePosition.row = 7;
			movePosition.column = 4;
			makeMove(movePosition);
			break;
		case SW:
			selectedFigure = figuresOnBoard[7][0];
			movePosition.row = 7;
			movePosition.column = 2;
			makeMove(movePosition);
			break;
		default:
			break;
	}
}

void Board::checkMoves(Colour colour)
{
	for (int i = 0; i <= 7; i++)
		for (int j = 0; j <= 7; j++)
		{
			if(figuresOnBoard[i][j] != nullptr)
				if(figuresOnBoard[i][j]->getColour() == colour)
					figuresOnBoard[i][j]->checkMoves();
		}			
}

bool Board::checkMateDetection(Colour colour)
{
	bool detected = true;
	for(int i = 0; i <= 7; i++)
		for (int j = 0; j <= 7; j++)
		{
			if(figuresOnBoard[i][j] != 0)
				if(figuresOnBoard[i][j]->getColour() == colour)
					if (figuresOnBoard[i][j]->getValidMoves()->size() != 0)
						detected = false;
		}
	return detected;
}

Figure* Board::getFig(Pos pos)
{
	if(pos.row < 0 || pos.row > 7 || pos.column < 0 || pos.column > 7)
		return nullptr;
	return figuresOnBoard[pos.row][pos.column];

}	

void Board::checkDetected(Colour colour)
{
	if (colour == WHITE)
		whiteChecked = true;
	else
		blackChecked = true;
}

void Board:: eraseCastling(Direction rookPos)
{
	switch(rookPos)
	{
		case NE:
			whiteRightCastling = false;
			break;
		case NW:
			whiteLeftCastling = false;
			break;
		case SE:
			blackRightCastling = false;
			break;
		case SW:
			blackLeftCastling = false;
			break;
		default:
			break;
	}
}

bool Board::getCastling(Direction rookPos)
{
	switch(rookPos)
	{
		case NE:
			return whiteRightCastling;
		case NW:
			return whiteLeftCastling;
		case SE:
			return blackRightCastling;
		case SW:
			return blackLeftCastling;
		default:
			return false;
	}
}