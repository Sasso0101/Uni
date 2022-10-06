#include <iostream>

using namespace std;

void swap(int* n1, int* n2) {
	int temp;
	temp = *n1;
	*n1 = *n2;
	*n2 = temp;
}

void sort(int& n1, int& n2, int& n3) {
	if (n1 > n2) swap(&n1, &n2);
	if (n2 > n3) swap(&n2, &n3);
	if (n1 > n2) swap(&n1, &n2);
}

int main() {
	int n1, n2, n3;
	cout << "Input three numbers: ";
	cin >> n1 >> n2 >> n3;
	sort(n1, n2, n3);
	cout << "n1: " << n1 << " n2: " << n2 << " n3: " << n3 << endl;
}
