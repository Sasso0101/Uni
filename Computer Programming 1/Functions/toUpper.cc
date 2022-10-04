#include <iostream>

using namespace std;

bool isLowercase(char c) {
	if (c >= 97 && c <= 122) return true;
	return false;
}

char toUppercase(char c) {
	return c-32;
}

int main() {
	char c;
	cout << "Insert lowercase character: ";
	cin >> c;
	if (isLowercase(c)) cout << "Uppercase: " << toUppercase(c) << 
endl;
	else cout << "The character is not valid" << endl;
}
