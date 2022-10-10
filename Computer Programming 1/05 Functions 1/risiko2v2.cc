#include <iostream>
#include <algorithm>

using namespace std;

int main() {
	srand(time(NULL));
	int striker1 = rand()%6+1;
	int striker2 = rand()%6+1;
	int defender1 = rand()%6+1;
	int defender2 = rand()%6+1;
	cout << "Striker dices: " << striker1 << " " << striker2 << endl;
	cout << "Defender dices: " << defender1 << " " << defender2 << endl;

	int strikerWins = 0;
	int defenderWins = 0;
	int ties = 0;
	
	int maxStriker = max(striker1, striker2);
	int maxDefender = max(defender1, defender2);
	int minStriker = min(striker1, striker2);
	int minDefender = min(defender1, defender2);

	if (maxStriker > maxDefender) strikerWins++;
	else if (maxStriker < maxDefender) defenderWins++;
	else ties++;
	if (minStriker > minDefender) strikerWins++;
	else if (minStriker < minDefender) defenderWins++;
	else ties++;
	cout << "Striker wins: " << strikerWins << endl;
	cout << "Defender wins: " << defenderWins << endl;
	cout << "Ties: " << ties << endl;
}
