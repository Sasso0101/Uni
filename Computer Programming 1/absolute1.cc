#include <iostream>

using namespace std;

int main() {
	int a, b;
	cout << "Insert numbers a and b: ";
	cin >> a >> b;
	int abs = a-b;
	if (abs < 0) abs = -1*abs;
	cout << "Result: " << abs << endl;
}
