#include <iostream>

using namespace std;

void revertNumber(int n) {
	do {
		cout << n%10;
		n = n/10;
	} while (n != 0);
	cout << endl;
}

int main() {
	int ones = 0;
	srand(time(NULL));
	for (int i=0; i<6; i++) {
		if ((rand()%6+1) == 1) ones++;
	}
	int guess;
	cout << "Guess the number of dice that turned out 1: ";
	cin >> guess;
	if (guess == ones) cout << "You guessed correctly!" << endl;
	else cout << "Your guess was wrong. The correct number is " << 
ones << "." << endl;
}
