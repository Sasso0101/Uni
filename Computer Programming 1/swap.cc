#include <iostream>

using namespace std;

int main() {
	int a, b;
	cout << "Insert two integer numbers: ";
	cin >> a >> b;
	int c = a;
	a = b;
	b = c;
	cout << "A: " << a << " B: " << b << endl;
}
