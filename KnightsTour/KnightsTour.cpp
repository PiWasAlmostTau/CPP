#include "Board.h"

/** ================================================================
 * Recursively solves the knights tour using brute force
 * Prints any solutions it finds.
 *
 * @param board - the board we’re working with (contains all moves-to-date)
 * @param row   - the row we’re going to attempt to place the knight on this move.
 * @param col   - the column we’re going to attempt place the knight on this move.
 * @param currentMoveNumber - the move# we’re making (1=first move)
 *
 * @return The number of solutions the given board and move leads to
 * =================================================================*/
int solveKnightsTour(Board board, int col, int row, int currentMoveNum = 1);

/** ================================================================
 * Solves the knights tour for every starting point on the grid, and returns the total
 * amount of solutions and the total number of magic boards
 *
 * @param totals - the board that will store and print the total number of solutions for each start point
 * @param magics - the board that will store and print the total number of semi magic boards for each start point
 * @param board - the board we’re working with (contains all moves-to-date)
 * =================================================================*/
void everyStartPoint(Board totals, Board magics, Board board);

//Static variables
int Board::size;
int Board::solutionNumber;
int Board::semiMagicBoards;


/*========================================================================
*	Main function
==========================================================================*/
int main() {
	print << "Welcome to the Knights Tour Solver!" << newL;

	print << "Please enter your board size: " << newL;
	int size;
	input >> size;
	Board b(size);
	input.clear();
	input.ignore(100, '\n');

	print << "Please enter your starting position, separated by a space (row col):" << newL;
	int x, y;
	input >> y >> x;
	
	solveKnightsTour(b, x - 1, y - 1);
	print << "Total Solutions: " << b.solutionNumber << newL;
	print << "Semi-magical Boards: " << b.semiMagicBoards << newL;

	/*Board t;
	Board m;
	everyStartPoint(t, m, b);*/

	input.clear();
	input.ignore(100, '\n');
	input.get();
	return 0;
}

/*========================================================================*/
int solveKnightsTour(Board board, int col, int row, int currentMoveNum) {

	board.addMove(col, row, currentMoveNum);
	if (currentMoveNum == board.size * board.size) {
		board.printSolvedBoard();
		return 1;
	}

	// +-1 steps first
	if (board.validMove(col + 1, row + 2))
		solveKnightsTour(board, col + 1, row + 2, currentMoveNum + 1);

	if (board.validMove(col + 1, row - 2))
		solveKnightsTour(board, col + 1, row - 2, currentMoveNum + 1);

	if (board.validMove(col - 1, row + 2))
		solveKnightsTour(board, col - 1, row + 2, currentMoveNum + 1);

	if (board.validMove(col - 1, row - 2))
		solveKnightsTour(board, col - 1, row - 2, currentMoveNum + 1);

	// +-2 steps first
	if (board.validMove(col + 2, row + 1))
		solveKnightsTour(board, col + 2, row + 1, currentMoveNum + 1);

	if (board.validMove(col + 2, row - 1))
		solveKnightsTour(board, col + 2, row - 1, currentMoveNum + 1);

	if (board.validMove(col - 2, row + 1))
		solveKnightsTour(board, col - 2, row + 1, currentMoveNum + 1);

	if (board.validMove(col - 2, row - 1))
		solveKnightsTour(board, col - 2, row - 1, currentMoveNum + 1);

	return 0;
}

/*========================================================================*/
void everyStartPoint(Board totals, Board magics, Board board) {
	for (int y = 0; y < board.size; y++) {
		for (int x = 0; x < board.size; x++) {
			board.solutionNumber = 0;
			board.semiMagicBoards = 0;
			solveKnightsTour(board, x, y);
			totals.addMove(x, y, board.solutionNumber);
			magics.addMove(x, y, board.semiMagicBoards);
		}
	}
	totals.printSolvedBoard();
	magics.printSolvedBoard();
}