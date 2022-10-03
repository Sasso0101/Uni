#include <iostream>

using namespace std;

int main() {
	char a;
	cout << "Insert a lowercase char: ";
	cin >> a;
	cout << (a-32) << endl;
	cout << "Uppercase char: " << (char)(a-32) << endl;
}
