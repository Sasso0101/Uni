using namespace std;
#include <iostream>

int main() {
	int hh, mm, ss;
	cout << "Input time in HH MM SS: ";
	cin >> hh >> mm >> ss;
	cout << "Seconds from midnight: " << (hh*60+mm)*60+ss << endl;
	return 0;
}
