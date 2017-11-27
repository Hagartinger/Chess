#ifndef GLOBALS_H
#define GLOBALS_H

namespace globals{

	const int SCREEN_WIDTH = 600;
	const int SCREEN_HEIGHT = 600;
	
	const int GRID_WIDTH = SCREEN_WIDTH  / 8;
	const int GRID_HEIGHT = SCREEN_HEIGHT  / 8;
	
}

struct Pos
{
	int row;
	int column;
};

enum Colour {WHITE, BLACK};
enum FigureType {PAWN, QUEEN, ROOK, KING, BISHOP, KNIGHT};

enum Direction { N = 0, NE = 1, E = 2, SE = 3, S = 4, SW = 5, W = 6, NW = 7};
#endif