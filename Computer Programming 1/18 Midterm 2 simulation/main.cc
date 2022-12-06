#include <iostream>
#include <fstream>
#include <string>
#include "dictionary.h"

using namespace std;

element* importProducts(element* stock, string filename) {
    ifstream file;
    file.open(filename, ios::in);
    while (!file.eof()) {
        string row;
        getline(file, row);
        if (row.length() == 0) continue;
        string code = row.substr(0, row.find(" "));
        string value = row.substr(row.rfind(" ")+1, row.length());
        stock = addToDictionary(stock, code, stoi(value));
    }
    return stock;
}

void decrementStockFromOrders(element* stock, string filename) {
    ifstream file;
    file.open(filename, ios::in);
    while (!file.eof()) {
        string row;
        getline(file, row);
        if (row.length() == 0) continue;
        size_t codePos = row.find_first_not_of(' ', row.find(' '));
        string code = row.substr(codePos, row.find(' ', codePos)-codePos);
        string value = row.substr(row.rfind(' ')+1, row.length());
        decrementElement(stock, code, stoi(value));
    }
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