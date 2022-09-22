using namespace std;
#include <iostream>

int main() {
	float temp;
	cout << "Input temperature in Fahrenheit: ";
	cin >> temp;
	cout << (temp - 32)/1.8<<"°C\n";
	return 0;
}
