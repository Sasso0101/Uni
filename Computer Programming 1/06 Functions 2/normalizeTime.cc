#include <iostream>

using namespace std;

int division(int dividend, int divisor, int& rest) {
	int remaining = dividend - divisor;
	if (divisor == 0) {
		cout << "Can't divide by 0!" << endl;
		return 0;
	} else if (dividend < divisor) {
		rest = dividend;
		return 0;
	} else if (remaining < divisor) {
		rest = remaining;
		return 1;
	} else {
		return 1+division(remaining, divisor, rest);
	}
}

int main() {
	int seconds, minutes, hours, rest = 0;
	cout << "Input seconds, minutes, hours: ";
	cin >> seconds >> minutes >> hours;
	minutes += division(seconds, 60, rest);
	seconds = rest;
	hours += division(minutes, 60, rest);
	minutes = rest;
	cout << "seconds: " << seconds << " minutes: " << minutes << " hours: " << hours << endl;
}
