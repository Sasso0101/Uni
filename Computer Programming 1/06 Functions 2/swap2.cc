#include <iostream>

using namespace std;

void swap(int* n1, int* n2) {
	int temp;
	temp = *n1 & 255; //Get last 8 bits
	*n1 = *n1 >> 8; //Shift to right by 8
	*n1 = (*n1 << 8) + (*n2 & 255); //Left shift by 8 and then add last 8 bits of n2
	*n2 = *n2 >> 8; //Shift to right by 8
	*n2 = (*n2 << 8) + temp; //Left shift by 8 and then add last 8 bits of n1
}

int main() {
	int n1, n2;
	cout << "Input two numbers: ";
	cin >> n1 >> n2;
	cout << "n1: " << n1 << " n2: " << n2 << endl;
	cout << "n1: " << bitset<16>(n1) << " n2: " << bitset<16>(n2) << endl;
	swap(&n1, &n2);
	cout << "n1: " << n1 << " n2: " << n2 << endl;
	cout << "n1: " << bitset<16>(n1) << " n2: " << bitset<16>(n2) << endl;
}
