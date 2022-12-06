#include "dictionary.h"

element* addToDictionary(element *dictionary, string key, int value) {
    element *newElement = new element;
    newElement->key = key;
    newElement->value = value;
    if (dictionary == NULL) newElement->next = NULL;
    else newElement->next = dictionary;
    return newElement;
};

element* findElement(element *dictionary, string key) {
    while(dictionary->key.compare(key) != 0) {
        dictionary = dictionary->next;
    }
    return dictionary;
};

void decrementElement(element *dictionary, string key, int decrement) {
    element *el = findElement(dictionary, key);
    el->value -= decrement;
};

void printDictionary(element *dictionary) {
    while (dictionary != NULL) {
        cout << dictionary->key << " " << dictionary->value << endl;
        dictionary = dictionary->next;
    }
};