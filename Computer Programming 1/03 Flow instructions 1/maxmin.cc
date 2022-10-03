#include <iostream>
#include <cmath>

using namespace std;

int main() {
	int a, b;
	cout << "Insert numbers a and b: ";
	cin >> a >> b;
	int minN, maxN;
	minN = min(a, b);
	maxN = max(a, b);
	cout << "Min: " << minN << " Max: " << maxN << endl;
}
