#include <iostream>

using namespace std;

bool isLowercase(char c) {
	if (c >= 97 && c <= 122) return true;
	return false;
}

void toUppercase(char *c) {
	*c=*c-32;
}

int main() {
	char c;
	char d;
	cout << "Insert lowercase character: ";
	cin >> c;
	if (isLowercase(c)) {
		toUppercase(&c);
		cout << "Uppercase: " << c << endl;
	}
	else cout << "The character is not valid" << endl;
}
