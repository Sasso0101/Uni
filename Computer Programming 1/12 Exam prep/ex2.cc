#include <iostream>

using namespace std;

int main() {
    cout << "Enter maximum term of the harmonic sequence: ";
	int n;
	while(!(cin >> n)) {
		cin.clear();
		cin.ignore(); 
		cout << "Input not an integer. Please, enter an integer number" << endl;
	}

	cout << " Entered value:" << n << endl;
}