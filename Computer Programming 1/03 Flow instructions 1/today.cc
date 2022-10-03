#include <iostream>

using namespace std;

int main() {
	int input;
	cout << "Insert day of number in week (1 = Monday and 7 = Sunday): ";
	cin >> input;
	if (input == 1) cout << "Monday" << endl;
	else if (input == 2) cout << "Tuesday" << endl;
	else if (input == 3) cout << "Wednesday" << endl;
	else if (input == 4) cout << "Thursday" << endl;
	else if (input == 5) cout << "Friday" << endl;
	else if (input == 6) cout << "Saturday" << endl;
	else if (input == 7) cout << "Sunday" << endl;
}
