#include <iostream>

using namespace std;

int main() {
	bool p, q;
	cout << "Insert P and Q as 1 or 0: ";
	cin >> p >> q;
	cout << "Result: " << (!p || q) << endl;
}
