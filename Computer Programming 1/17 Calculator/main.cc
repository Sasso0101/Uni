#include <iostream>
#include <string>
#include <stdlib.h>
#include "calculator.h"

using namespace std;

int main() {
    string input;
    do {
        cout << "Enter input: ";
        cin >> input;
        if (input.compare("+") == 0) operation('+');
        else if (input.compare("-") == 0) operation('-');
        else if (input.compare("s") == 0) swapValues();
        else if (input.compare("p") == 0) print();
        else {
            char *ptr = NULL;
            int n = strtol(input.c_str(), &ptr, 10);
            if (!*ptr) {
                pushNumber(n);
            }
        }
    } while (input.compare("q") != 0);
}