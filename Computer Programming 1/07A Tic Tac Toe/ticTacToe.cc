#include <iostream>
#include <time.h>

using namespace std;

const int SIZE = 3;

enum player {None, Player, Computer};

void resetMatrix(player [][SIZE]);
void printMatrix(const player [][SIZE]);
bool move(player [][SIZE], player, int, int);
void computerMove(int &, int &);
int checkWin(player [][SIZE]);

int main() {
	player playboard[3][3];
	int row = 0;
	int column = 0;
	int outcome = None;
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
		outcome = checkWin(playboard);
		if (outcome == Player) {
			cout << "You win!" << endl;
			break;
		} else if (outcome == -1) {
			cout << "Tie!" << endl;
			break;
		}
		do {
			computerMove(row, column);
		} while (!move(playboard, Computer, row, column));
		printMatrix(playboard);
		outcome = checkWin(playboard);
		if (outcome == Computer) {
			cout << "You lost!" << endl;
			break;
		} else if (outcome == -1) {
			cout << "Tie!" << endl;
			break;
		}
	} while(true);
}

/* Resets matrix to initial state */
void resetMatrix(player matrix[][SIZE]) {
	for(int r = 0; r < SIZE; r++) {
		for(int c = 0; c < SIZE; c++) {
			matrix[r][c] = None;
		}
	}
}

/* Print matrix on screen */
void printMatrix(const player matrix[][SIZE]) {
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
bool move(player matrix[][SIZE], player player, int row, int column) {
	if (matrix[row][column] == None && row < SIZE && column < SIZE && row >= 0 && column >= 0) {
		matrix[row][column] = player;
		return true;
	} else {
		return false;
	}
}

/* Checks for winner
	Returns None if no winner
	Returns Player if player wins
	Returns Computer if computer wins
	Returns -1 if tie
 */
int checkWin(player matrix[][SIZE]) {
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
	// Check tie
	bool tie = true;
	for (int r=0; r<SIZE; r++) {
		for (int c=0; c<SIZE; c++) {
			if (matrix[r][c] == None) {
				tie = false;
			}
		}
	}
	if (tie) return -1;
	return None;
}

/* Generates a random computer move
	Passes generated move by reference
 */
void computerMove(int& row, int& column) {
	row = rand() % SIZE;
	column = rand() % SIZE;
}