#include <iostream>
#include <cstdlib>
using namespace std;

int main() {
	int n;
	cout << "Numer of lines: ";
	cin >> n;
	int totalAsterisks = 1+2*(n-1);
	// i = number of asterisks
	for (int i = 1; i <= totalAsterisks; i+=2) {
		// Spaces before *
		for (int j = 0; j < (totalAsterisks-i)/2; j++) {
			cout << " ";
		}
		// Asterisks
		for (int j = 0; j < i; j++) {
			cout << "*";
		}
		for (int j = 0; j < (totalAsterisks-i)/2; j++) {
			cout << " ";
		}
		cout << endl;
	}
}
