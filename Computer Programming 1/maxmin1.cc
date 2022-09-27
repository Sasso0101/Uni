#include <iostream>

using namespace std;

int main() {
	int a, b;
	cout << "Insert numbers a and b: ";
	cin >> a >> b;
	int minN, maxN;
	if (a > b) {
		minN = b;
		maxN = a;
	} else {
		minN = a;
		maxN = b;
	}
	cout << "Min: " << minN << " Max: " << maxN << endl;
}
