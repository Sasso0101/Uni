#ifndef STACK_H
#define STACK_H

#include <iostream>

struct element {
    int value;
    element *next;
};

element* push(int value, element *top);
element* pop(int &value, element *top);
int getValue(element *top);
element* getNext(element *top);
void clear(element* top);

#endif