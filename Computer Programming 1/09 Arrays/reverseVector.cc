#include <iostream>

using namespace std;

int main() {
    int v[10] = {1,2,3,4,5,6,7,8,9,10};
    for(int i = (sizeof(v)/sizeof(int))-1; i >= 0; i--) {
        cout << v[i] << " ";
    }
}