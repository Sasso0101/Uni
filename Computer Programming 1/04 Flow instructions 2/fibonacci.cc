#include <iostream>

using namespace std;

int main() {
	int n = 0;
	int n1 = 0;
	int n2 = 0;
	cout << "How many numbers do you want to print? ";
	cin >> n;
	if (n == 0) cout << 0 << endl;
	else if (n == 1) cout << 1 << endl;
	else {
		for (int i=0; i<n; i++) {
			cout << n2 << endl;
			if (n2 == 0) n2 = 1;
			else {
				int oldn1 = n1;
				n1 = n2;
				n2 = oldn1+n2;
			}
		}
	}
}
