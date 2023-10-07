#include <iostream>

using namespace std;

int main() {
    // Color r = blue;
    // switch(r)
    // {
    //     case red  : std::cout << r << endl;   break;
    //     case green: std::cout << r << endl; break;
    //     case blue : std::cout << r << endl;  break;
    // }

    int n1;
	cin >> n1;
    n1 = n1 << 2;
	cout << "n1: " << bitset<4>(n1) << endl;
}