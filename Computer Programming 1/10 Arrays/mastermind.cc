#include <iostream>
#include <string>
#include <time.h>

using namespace std;

const int TURNS = 12;
const int CODELENGTH = 4;
const int BOARDWIDTH = CODELENGTH*2;

// 0: Default, 1: Red, 2: Green, 3: Yellow, 4: Blue, 5: Magenta, 6: Cyan
const int COLORS[7] = {39, 31, 32, 33, 34, 35, 36};

/* Sets color of output
    Colors from 1-6
    Passing 0 resets the color
*/
void setColor(const int color) {
    cout << "\033[" << COLORS[color] << "m";
} 

void printBoard(const int board[TURNS][BOARDWIDTH]) {
    cout << "------------------------" << endl;
    for (int r = 0; r < TURNS; r++) {
        // Print colors
        for (int c = 0; c < CODELENGTH; c++) {
            cout << "| ";
            setColor(board[r][c]);
            cout << board[r][c];
            setColor(0);
            cout << " ";
        }
        cout << "| ";
        // Print hints
        for (int c = CODELENGTH; c < BOARDWIDTH; c++) {
            if (board[r][c] == 1) setColor(1);
            else if (board[r][c] == 2) setColor(3);
            cout << "o";
            setColor(0);
        }
        cout << " |" << endl;
    }
    cout << "------------------------" << endl;
}

/* Generates random code */
void generateCode(int code[CODELENGTH]) {
    for (int i = 0; i < CODELENGTH; i++) {
        code[i] = (rand() % 6)+1;
    }
}

bool validateTurn(int board[TURNS][BOARDWIDTH], const int code[CODELENGTH], int currentTurn) {
    int won = true;
    int incorrectColorsCounter[6] = {};
    // Create counter for each color in correct code
    cout << "Code: ";
    for (int i = 0; i < CODELENGTH; i++) {
        cout << code[i];
        incorrectColorsCounter[code[i]-1]++;
    }
    cout << endl;
    // Check correct guesses in correct place
    for (int i = 0; i < CODELENGTH; i++) {
        if (board[currentTurn][i] == code[i]) {
            incorrectColorsCounter[board[currentTurn][i]-1]--;
            // Red color
            board[currentTurn][i+CODELENGTH] = 1;
        } else {
            won = false;
        }
    }
    // Check correct guesses in wrong place
    for (int i = 0; i < CODELENGTH; i++) {
        if (
            board[currentTurn][i] != code[i] &&
            incorrectColorsCounter[board[currentTurn][i]-1] > 0
        ) {
            // Gray color
            board[currentTurn][i+CODELENGTH] = 2;
            incorrectColorsCounter[board[currentTurn][i]-1]--;
        }
    }
    return won;
}

int main() {
    int board[TURNS][BOARDWIDTH] = {};
    int code[CODELENGTH] = {};
    // Initialize RNG
    srand(time(NULL));
    generateCode(code);
    int currentTurn = -1;
    int won = false;

    cout << "Welcome to mastermind!" << endl;
    do {
        currentTurn++;
        cout << "Enter sequence of 4 numbers (1-6): ";
        cin >> board[currentTurn][0] >> board[currentTurn][1] >> board[currentTurn][2] >> board[currentTurn][3];
        if (validateTurn(board, code, currentTurn)) won = true;
        printBoard(board);
    } while(currentTurn < 11 && !won);
    if (won) cout << "You won!" << endl;
    else cout << "You lost!" << endl;
    return 0;
}