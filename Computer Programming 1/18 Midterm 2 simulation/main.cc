#include <iostream>
#include <fstream>
#include <string>
#include "dictionary.h"

using namespace std;

ifstream openFile(string filename) {
    ifstream file;
    file.open(filename, ios::in);
    if (file.fail()) throw "Can't open file!";
    return file;
}

/**
 * Split string using space as delimiting
 * character
*/
void splitString(string s, string splittedString[3]) {
    int i = 0;
    while (i < 3) {
        size_t firstChar = s.find_first_not_of(' ');
        size_t lastChar = s.find(' ', firstChar);
        splittedString[i] = s.substr(firstChar, lastChar-firstChar);
        s.erase(0, lastChar);
        i++;
    }
}

/**
 * Import products from file and adds them to
 * stock
*/
element* importProducts(element* stock, string filename) {
    ifstream file = openFile(filename);
    string row;
    while (getline(file, row)) {
        if (row.length() == 0) continue;
        string splittedString[3];
        splitString(row, splittedString);
        stock = addToDictionary(stock, splittedString[0], stoi(splittedString[2]));
    }
    file.close();
    return stock;
}

/**
 * Decrements stock based on placed order
 * quantity
*/
void decrementStockFromOrders(element* stock, string filename) {
    ifstream file = openFile(filename);
    string row;
    while (getline(file, row)) {
        if (row.length() == 0) continue;
        string splittedString[3];
        splitString(row, splittedString);
        decrementElement(stock, splittedString[1], stoi(splittedString[2]));
    }
    file.close();
}

int main(int argc, char *argv[]) {
    try {
        element* stock = NULL;
        if (argc != 3) throw "Number of arguments not correct!";
        stock = importProducts(stock, argv[1]);
        decrementStockFromOrders(stock, argv[2]);
        printDictionary(stock);
    } catch(const char* error) {
        cout << error << endl;
    }
}