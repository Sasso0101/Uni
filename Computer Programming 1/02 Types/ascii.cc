#include <iostream>

using namespace std;

int main() {
	char a;
	cout << "Insert a char: ";
	cin >> a;
	a += 10;
	cout << "Char +10: " << (char)a << "\nChar -10: " << (char)(a-10) << endl;
}
