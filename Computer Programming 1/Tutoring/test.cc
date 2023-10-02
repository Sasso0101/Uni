#include <iostream>

using namespace std;

enum Color { red = 1, green = 4, blue };
int main() {
    Color r = blue;
    switch(r)
    {
        case red  : std::cout << r << endl;   break;
        case green: std::cout << r << endl; break;
        case blue : std::cout << r << endl;  break;
    }
}