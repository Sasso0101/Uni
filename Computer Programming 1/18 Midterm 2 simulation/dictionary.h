#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <iostream>
#include <string>

using namespace std;

struct element {
    string key;
    int value;
    element *next;
};

element* addToDictionary(element *dictionary, string key, int value);
element* findElement(element *dictionary, string key);
void decrementElement(element *dictionary, string key, int decrement);
void printDictionary(element *dictionary);

#endif