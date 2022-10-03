#include <iostream>
#include <cmath>

using namespace std;

int main() {
	int x1, x2;
	cout << "Input ASCII range: ";
	cin >> x1 >> x2;
	if (x1 <= x2 && ((x1 >= 65 && x2 <= 90) || (x1 >= 97 && x2 <=122))) {
		for (int i = x1; i<= x2; i++) {
			cout << (char)i << " ";
		}
	} else {
		cout << "The range is not valid!";
	}
	cout << endl;
}
