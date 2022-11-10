#include <iostream>

using namespace std;

void transposeMatrix(int matrix[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = i+1; j < 3; j++) {
            int temp = matrix[i][j];
            matrix[i][j] = matrix[j][i];
            matrix[j][i] = temp;
        }
    }
}

void scalarMultiplication(int matrix[3][3], int scalar) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            matrix[i][j] = matrix[i][j]*scalar;
        }
    }
}

void printMatrix(const int matrix[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

int main() {
    int matrix[3][3] = {};

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cout << "Enter value [" << i << ", " << j << "]: ";
            cin >> matrix[i][j];
        }
    }
    printMatrix(matrix);
    cout << "Matrix transpose" << endl;
    transposeMatrix(matrix);
    printMatrix(matrix);
    cout << "Enter scalar: ";
    int scalar = 0;
    cin >> scalar;
    scalarMultiplication(matrix, scalar);
    printMatrix(matrix);
}