#include <iostream>

using namespace std;

// op = 0: Or, 1: And, 2: XOR
void table(int op) {
	bool a = false;
	bool b = false;
	cout << "|  A  |  B  | Out |\n";
	for (int i = 0; i<2; i++) {
		for (int j = 0; j<2; j++) {
			int res = 0;
			if (op == 0) res = (int)(a || b);
			else if (op==1) res = (int)(a && b);
			else if (op==2) res = (int)(a != b);
			cout << "|  " << a << "  |  " << b << "  |  " << res << "  |\n";
			a = !a;
		}
		b = !b;
	}
}

int main() {
	cout << " --- OR ---\n";
	table(0);
	cout << "\n\n --- AND ---\n";
	table(1);
	cout << "\n\n --- XOR ---\n";
	table(2);
}
