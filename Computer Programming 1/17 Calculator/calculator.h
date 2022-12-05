#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <iostream>
#include "stack.h"

using namespace std;

extern element* top;

void pushNumber(int n);
void operation(char operation);
void swapValues();
void print();

#endif