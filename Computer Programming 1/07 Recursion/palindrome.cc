#include <iostream>

using namespace std;

// Returns number of digits of n
int length(int n) {
    int i = 1;
    while (n >= 10) {
        n /= 10;
        i++;
    }
    cout << "Length: " << i << endl;
    return i;
}

int firstDigit(int n) {
    while (n >= 10) {
        n /= 10;
    }
    return n;
}

int lastDigit(int n) {
    return n % 10;
}

bool palindrome(int n, int len, int i = 1) {
    // If we have only one digit return true
    if (i > len/2) {
        return true;
    }
    // If first and last digits are equal check the inner
    if (firstDigit(n) == lastDigit(n)) {
        return palindrome(n, len, i+1);
    } else {
        return false;
    }
}

int main() {
    int n;
	cout << "Input a decimal number: ";
    cin >> n;
    cout << palindrome(n, length(n)) << endl;
}
