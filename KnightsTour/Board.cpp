#include "Board.h"

/*=================================================================
*	Constructor for the Board class
*	@param size - the size to make each side of the board
==================================================================*/
Board::Board(int size) {
	for (int y = 0; y < MAX_SIZE; y++) {
		for (int x = 0; x < MAX_SIZE; x++) {
			board[x][y] = 0;
		}
	}
	this->size = size;
}

/*=================================================================
*	Checks if a move is valid for the current board
*	@param x - x position of possible move
*	@param y - y position of possible move
*	@return bool, true if the move is valid, false if not
==================================================================*/
bool Board::validMove(int x, int y) {
	if (board[x][y] != 0 || outOfBounds(x) || outOfBounds(y))
		return false;
	else
		return true;
}

/*=================================================================
*	Checks if a position is out of bounds (outside the board)
*	@param n - number to check against bounds
*	@return bool, true if out of bounds, false if not
==================================================================*/
bool Board::outOfBounds(int n) {
	if (n < 0 || n > size - 1)
		return true;
	else
		return false;
}

/*=================================================================
*	Adds a given move to the current board
*	@param x - x position of the move
*	@param y - y position of the move
*	@param moveNum - current move nummber
==================================================================*/
void Board::addMove(int x, int y, int moveNum) {
	board[x][y] = moveNum;
}

/*=================================================================
*	Increments the solution number and prints the current solved board,
*	also checks if the board is semi-magical and prints and increments if it is
==================================================================*/
void Board::printSolvedBoard() {
	solutionNumber++;

	print << "Solution #" << solutionNumber << ":" << newL;
	for (int y = 0; y < size; y++) {
		for (int x = 0; x < size; x++) {
			int num = board[x][y];
			print << ((num < 10) ? "0" : "") << num << " ";
		}
		print << newL;
	}
	if (isSemiMagic()) {
		print << "Semi Magical Board!!" << newL;
		semiMagicBoards++;
	}
	print << newL;
}

/*=================================================================
*	Checks if the current board is a semi-magical board, by comparing
*	the sum of the first row to the sum of every other row and every
*	other column.
*	@return bool, true if the board is semi-magical, false if not
==================================================================*/
bool Board::isSemiMagic() {
	int magicNum = 0;
	for (int x = 0; x < size; x++)
		magicNum += board[x][0];

	for (int y = 1; y < size; y++) {
		int sum = 0;
		for (int x = 0; x < size; x++)
			sum += board[x][y];
		if (sum != magicNum)
			return false;
	}

	for (int x = 0; x < size; x++) {
		int sum = 0;
		for (int y = 0; y < size; y++)
			sum += board[x][y];
		if (sum != magicNum)
			return false;
	}

	return true;
}