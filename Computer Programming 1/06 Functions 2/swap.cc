#include <iostream>

using namespace std;

void swap(double* n1, double* n2) {
	double temp;
	temp = *n1;
	*n1 = *n2;
	*n2 = temp;
}

int main() {
	double n1, n2;
	cout << "Input two double numbers: ";
	cin >> n1 >> n2;
	swap(&n1, &n2);
	cout << "n1: " << n1 << " n2: " << n2 << endl;
}
