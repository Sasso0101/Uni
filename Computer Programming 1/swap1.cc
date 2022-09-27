#include <iostream>

using namespace std;

int main() {
	int a, b;
	cout << "Insert two integer numbers: ";
	cin >> a >> b;
	a = a+b;
	b = a-b;
	a = a-b;
	cout << "A: " << a << " B: " << b << endl;
}
