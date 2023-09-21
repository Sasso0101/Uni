#include <bitset>
#include <iostream>

using namespace std;

int main() {
    char a = -12;
    char b = 1;
    bitset<8> x(a);
    cout << x << '\n';
    bitset<8> y(b);
    cout << y << '\n';

    char c = a & b;
    bitset<8> z(c);
    cout << z << '\n';
}