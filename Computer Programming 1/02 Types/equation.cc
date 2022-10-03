#include <iostream>
#include <cmath>

using namespace std;

void equation(float a, float b, float c) {
	float delta = (b*b)-(4*a*c);
	if (delta >= 0) {
		delta = sqrt(delta);
		float x1 = (-b+delta)/2*a;
		float x2 = (-b-delta)/2*a;
		cout << "x1: " << x1 << " x2: " << x2 << endl;
	} else {
		cout << "Delta is negative!\n";
	}
}

int main() {
	float a, b, c;
	cout << "Insert 3 real numbers a, b, c: ";
	cin >> a >> b >> c;
	equation(a, b, c);
}
