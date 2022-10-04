#include <iostream>
#include <cctype>

using namespace std;

void toUpperVoid(char *c) {
	*c = toupper(*c);
}

int main() {
	char c;
	cout << "Insert lowercase character: ";
	cin >> c;
	if (islower(c)) {
		toUpperVoid(&c);
		cout << "Uppercase: " << c << endl;
	} else cout << "The character is not valid" << endl;
}
