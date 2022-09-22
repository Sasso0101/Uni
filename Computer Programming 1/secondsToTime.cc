using namespace std;
#include <iostream>

int main() {
	int input;
	cout << "Input time in seconds: ";
	cin >> input;
	int ss = input % 60;
	input = input - ss;
	int mm = (input % 3600)/60;
	input = input - mm*60;
	int hh = input / 3600;
	cout << "Time: " << hh << ":" << mm << ":" << ss << endl;
	return 0;
}
