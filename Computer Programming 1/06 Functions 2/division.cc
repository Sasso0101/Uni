#include <iostream>

using namespace std;

int division(int dividend, int divisor, int& rest) {
	int remaining = dividend - divisor;
	if (divisor == 0) {
		cout << "Can't divide by 0!" << endl;
		return 0;
	} else if (remaining < divisor) {
		rest = remaining;
		return 1;
	} else {
		return 1+division(remaining, divisor, rest);
	}
}

int main() {
	int dividend, divisor, rest = 0;
	cout << "Input divident and divisor: ";
	cin >> dividend >> divisor;
	int quotient = division(dividend, divisor, rest);
	cout << "quotient: " << quotient << " rest: " << rest << endl;
}
