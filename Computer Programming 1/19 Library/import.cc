#include <iostream>
#include <vector>
#include "library.h"

using namespace std;

int main(int argc, char* argv[]) {
    vector<book*> library;
    importBooks(library, argv[1]);
    printBooks(library, true);
}