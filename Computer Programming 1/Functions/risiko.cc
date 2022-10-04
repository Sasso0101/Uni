#include <iostream>

using namespace std;

int main() {
	srand(time(NULL));
	int striker = rand()%6+1;
	int defender = rand()%6+1;
	cout << "Striker: " << striker << " defender: " << defender << 
endl;
	if (striker > defender) cout << "The winner is: striker!" << 
endl;
	else if (striker < defender) cout << "The winner is: defender!" << endl;
	else cout << "Game tied" << endl;
}
