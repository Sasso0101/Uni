#include <iostream>

using namespace std;

void printVector(int v[], int size) {
    for(int i = 0; i < size; i++) {
        cout << v[i] << ", ";
    }
}

int main() {
    int v1[10] = {1,2,3,4,5,6,7,8,9,10};
    int v2[10] = {10,9,8,7,6,5,4,3,2,1};
    int product[10] = {0};
    for(int i = 0; i < sizeof(v1)/sizeof(int); i++) {
        product[i] = v1[i]*v2[i];
    }
    printVector(product, sizeof(product)/sizeof(int));
}