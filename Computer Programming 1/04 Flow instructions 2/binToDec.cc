#include <iostream>
#include <cmath>

using namespace std;

int main() {
	int n = 0;
	int exp = 0;
	int res = 0;
	cout << "Insert number in binary: ";
	cin >> n;
	while (n != 0) {
		res += ldexp(n%10, exp); // n%10*2^x
		exp++;
		n = n/10;
	}
	cout << res << endl;
}
