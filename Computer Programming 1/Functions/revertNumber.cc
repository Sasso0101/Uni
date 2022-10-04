#include <iostream>

using namespace std;

void revertNumber(int n) {
	do {
		cout << n%10;
		n = n/10;
	} while (n != 0);
	cout << endl;
}

int main() {
	int n;
	cout << "Insert number: ";
	cin >> n;
	revertNumber(n);
}
