#include <iostream>
#include <cctype>

using namespace std;

int main() {
	char c;
	cout << "Insert lowercase character: ";
	cin >> c;
	if (islower(c)) cout << "Uppercase: " << (char)toupper(c) << 
endl;
	else cout << "The character is not valid" << endl;
}
