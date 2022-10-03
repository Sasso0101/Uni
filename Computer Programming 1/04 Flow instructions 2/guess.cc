#include <iostream>
#include <cstdlib>
using namespace std;

int main() {
	srand(time(NULL));
	int random = rand() % 10 + 1;
	int guess = -1;
	do {
		cout << "Guess the number: ";
		cin >> guess;
	} while(guess != random);
	cout << "You guessed correctly!" << endl;
}
