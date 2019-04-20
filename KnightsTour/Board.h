#pragma once
#include <iostream>
#define print std::cout
#define newL std::endl
#define input std::cin

const int MAX_SIZE = 5;

class Board
{
public:
	static int size;
	static int solutionNumber;
	static int semiMagicBoards;

	Board(int size);
	bool validMove(int x, int y);
	bool outOfBounds(int n);
	void addMove(int x, int y, int moveNum);
	void printSolvedBoard();
	bool isSemiMagic();

private:

	int board[MAX_SIZE][MAX_SIZE];
};

