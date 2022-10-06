#include <iostream>
#include <climits>

using namespace std;

int max(int n1, int n2 = INT_MIN, int n3 = INT_MIN, int n4 = INT_MIN, int n5 = INT_MIN) {
	int max = n1;
	if (n2 > max) max = n2;
	if (n3 > max) max = n3;
	if (n4 > max) max = n4;
	if (n5 > max) max = n5;
	return max;
}

int main() {
	int n1, n2, n3, n4, n5;
	cout << "Input 5 numbers: ";
	cin >> n1 >> n2 >> n3 >> n4 >> n5;
	int maxN = max(n1, n2, n3, n4, n5);
	cout << "Max: " << maxN << endl;
}
