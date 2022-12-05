#include "calculator.h"

element *top = NULL;

void popTwo(int &n1, int &n2) {
    top = pop(n1, top);
    top = pop(n2, top);
}

void pushNumber(int n) {
    top = push(n, top);
};

void operation(char operation) {
    int n1, n2;
    popTwo(n1, n2);
    int res;
    switch (operation) {
        case '+':
            res = n2 + n1;
            break;
        case '-':
            res = n2 - n1;
            break;
    }
    top = push(res, top);
};

void swapValues() {
    int n1, n2;
    popTwo(n1, n2);
    top = push(n1, top);
    top = push(n2, top);
};

void print() {
    element* current = top;
    cout << "Content of stack: ";
    while(current != NULL) {
        cout << getValue(current) << " ";
        current = getNext(current);
    }
    cout << endl;
};