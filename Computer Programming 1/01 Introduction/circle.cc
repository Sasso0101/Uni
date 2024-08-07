#include <iostream>
#include <cmath>

using namespace std;

const float pi = 3.1415;

float circumference(float r);
int main() {
	float radius;
	cout << "Input radius: ";
	cin >> radius;
	cout << "Circumference: " << circumference(radius) << "\nArea: " << pi*pow(radius, 2) << endl;
	return 0;
}

float circumference(float r) {
	return 2*pi*r;
}
