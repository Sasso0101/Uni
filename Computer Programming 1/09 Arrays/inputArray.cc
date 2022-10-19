#include <iostream>

using namespace std;

void printVector(int v[][100], int rows, int cols) {
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            cout << v[i][j] << ", ";
        }
        cout << endl;
    }
}

int main() {
    int rows = 0;
    int cols = 0;
    cout << "Insert number of rows of vector: ";
    cin >> rows;
    cout << "Insert number of cols of vector: ";
    cin >> cols;

    int array[100][100] = {};
    for(int i = 0; i < rows; i++) {
        cout << "Insert values of row " << i << ": " << endl;
        for(int j = 0; j < cols; j++) {
            cin >> array[i][j];
        }
    }
    printVector(array, rows, cols);
}