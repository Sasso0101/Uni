#include <iostream>

using namespace std;

void printBinary(int n) {
    if (n == 0) {
        return;
    }
    // Take last bit
    printBinary(n / 2);
	cout << (n % 2);
}

int main() {
    int n;
	cout << "Input a decimal number: ";
    cin >> n;
	printBinary(n);
    cout << endl;
}
