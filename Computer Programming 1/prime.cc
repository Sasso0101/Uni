#include <iostream>
#include <cmath>

using namespace std;

bool isPrime(int n) {
	bool isDividable = false;
	int current = 1;
	if (n == 1) return true;
	while (current < n/2 && !isDividable) {
		current++;
		if (n%current == 0) isDividable = true;
	}
	if (isDividable) return false;
	else return true;
}

int main() {
	int current = 0;
	int input = 0;
	bool numbersFound = false;
	cout << "Input number: ";
	cin >> input;
	while (current < input/2 && !numbersFound) {
		current += 1;
		if (isPrime(current) && isPrime(input-current)) {
			numbersFound = true;
		}
	}
	if (numbersFound) cout << "Numbers: " << current << " + " << input-current << 
endl;
	else cout << "No numbers found" << endl;
}
