#include <iostream>

using namespace std;

int main() {
	int a, b;
	cout << "Insert numbers a and b: ";
	cin >> a >> b;
	int abs = (a-b);
	int sign = (a-b)<0;
	int res = abs + (sign*abs*(-2));
	cout << "Res: " << res << endl;
}
