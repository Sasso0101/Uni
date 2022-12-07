#include "dictionary.h"

/**
 * Add element to dictionary
*/
element* addToDictionary(element *dictionary, string key, int value) {
    element *newElement = new element;
    newElement->key = key;
    newElement->value = value;
    if (dictionary == NULL) newElement->next = NULL;
    else newElement->next = dictionary;
    return newElement;
};

/**
 * Find element in dictionary by key
*/
element* findElement(element *dictionary, string key) {
    while(dictionary != NULL && dictionary->key.compare(key) != 0) {
        dictionary = dictionary->next;
    }
    if (dictionary == NULL) throw "Can't find element!";
    return dictionary;
};

/**
 * Decrement element value
*/
void decrementElement(element *dictionary, string key, int decrement) {
    element *el = findElement(dictionary, key);
    el->value -= decrement;
};

/**
 * Prints content of dictionary
*/
void printDictionary(element *dictionary) {
    while (dictionary != NULL) {
        cout << dictionary->key << " " << dictionary->value << endl;
        dictionary = dictionary->next;
    }
};