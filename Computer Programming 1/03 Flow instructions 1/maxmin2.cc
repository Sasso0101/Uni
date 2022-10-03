#include <iostream>

using namespace std;

int main() {
	int a, b;
	cout << "Insert numbers a and b: ";
	cin >> a >> b;
	int minN, maxN;
	bool check = a > b;
	maxN = (a*check)+(b*!check);
	minN = (a*!check)+(b*check);
	cout << "Min: " << minN << " Max: " << maxN << endl;
}
