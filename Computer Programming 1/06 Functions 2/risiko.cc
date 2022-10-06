#include <iostream>
#include <cstdlib>

using namespace std;

void swap(int& n1, int& n2) {
	int temp;
	temp = n1;
	n1 = n2;
	n2 = temp;
}

void sort(int& n1, int& n2, int& n3) {
	if (n1 > n2) swap(n1, n2);
	if (n2 > n3) swap(n2, n3);
	if (n1 > n2) swap(n1, n2);
}

void compare(int& a1, int& a2, int& a3, int& d1, int& d2, int& d3, int& attackerWins) {
	if (a1 > d1) attackerWins++;
	if (a2 > d2) attackerWins++;
	if (a3 > d3) attackerWins++;
}

int main() {
	int a1, a2, a3, d1, d2, d3;
	srand(time(NULL));
	a1 = rand()%6 + 1;
	a2 = rand()%6 + 1;
	a3 = rand()%6 + 1;
	d1 = rand()%6 + 1;
	d2 = rand()%6 + 1;
	d3 = rand()%6 + 1;
	sort(a1, a2, a3);
	sort(d1, d2, d3);
	cout << a1 << " " << a2 << " " << a3 << endl;
	cout << d1 << " " << d2 << " " << d3 << endl;
	int attackerWins=0;
	compare(a1, a2, a3, d1, d2, d3, attackerWins);
	cout << "Attacker wins: " << attackerWins << endl << "Defender wins: " << 
3-attackerWins << endl;
}
