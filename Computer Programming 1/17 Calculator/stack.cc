#include "stack.h"

element* push(int value, element *top = NULL) {
    element* newElement = new element;
    if (top != NULL) newElement->next = top;
    newElement->value = value;
    return newElement;
};

element* pop(int &value, element *top) {
    value = top->value;
    element* next = top->next;
    delete top;
    return next;
};

int getValue(element *top) {
    return top->value;
}

element* getNext(element *top) {
    return top->next;
};

void clear(element* top) {
    while(top != NULL) {
        element* next = top->next;
        delete top;
        top = next;
    }
};