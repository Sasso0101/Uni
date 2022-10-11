#include <iostream>
#include <time.h>

using namespace std;

const int SIZE = 3;

enum players {None, Computer, Player};

void resetMatrix(players [][SIZE]);
void printMatrix(const players [][SIZE]);
bool move(players [][SIZE], players, int, int);
void computerMove(int &, int &);
players checkWin(players [][SIZE]);

int main() {
	players playboard[3][3];
	int row = 0;
	int column = 0;
	// Initialize RNG
	srand (time(NULL));
	resetMatrix(playboard);
	do {
		// Executes until player enters a valid move
		do {
			cout << "Enter your move (row column): ";
			cin >> row >> column;
			// Decrement row and col (indexes start at 0)
			row--;
			column--;
		} while (!move(playboard, Player, row, column));
		printMatrix(playboard);
		if (checkWin(playboard) == Player) {
			cout << "You win!" << endl;
			break;
		}
		do {
			computerMove(row, column);
		} while (!move(playboard, Computer, row, column));
		printMatrix(playboard);
		if (checkWin(playboard) == Computer) {
			cout << "You lost!" << endl;
			break;
		}
	} while(true);
}

/* Resets matrix to initial state */
void resetMatrix(players matrix[][SIZE]) {
	for(int r = 0; r < SIZE; r++) {
		for(int c = 0; c < SIZE; c++) {
			matrix[r][c] = None;
		}
	}
}

/* Print matrix on screen */
void printMatrix(const players matrix[][SIZE]) {
	cout << endl << "---------" << endl;
	for (int r=0; r<SIZE; r++) {
		for (int c=0; c<SIZE; c++) {
			cout << "|";
			switch (matrix[r][c]) {
				case None:
					cout << " ";
					break;
				case Computer:
					cout << "O";
					break;
				case Player:
					cout << "X";
					break;
			}
			cout << "|";
		}
		cout << endl << "---------" << endl;
	}
}

/* Places a move on the board
   Returns false if the move is not valid
  */
bool move(players matrix[][SIZE], players player, int row, int column) {
	if (matrix[row][column] == None && row < SIZE && column < SIZE && row >= 0 && column >= 0) {
		matrix[row][column] = player;
		return true;
	} else {
		return false;
	}
}

/* Checks for winner
	Returns None if no winner, else returns player
*/
players checkWin(players matrix[][SIZE]) {
	// Check rows
	for (int i=0; i<SIZE; i++) {
		if (matrix[i][0] != None && matrix[i][0] == matrix[i][1] && matrix[i][1] == matrix[i][2]) {
			return matrix[i][0];
		}
	}
	// Check columns
	for (int i=0; i<SIZE; i++) {
		if (matrix[0][i] != None && matrix[0][i] == matrix[1][i] && matrix[1][i] == matrix[2][i]) {
			return matrix[0][i];
		}
	}
	// Check diagonals
	if (matrix[0][0] != None && matrix[0][0] == matrix[1][1] && matrix[1][1] == matrix[2][2]) {
		return matrix[0][0];
	}
	if (matrix[0][2] != None && matrix[0][2] == matrix[1][1] && matrix[1][1] == matrix[2][0]) {
		return matrix[0][2];
	}
	return None;
}

/* Generates a random computer move */
void computerMove(int& row, int& column) {
	row = rand() % SIZE;
	column = rand() % SIZE;
}