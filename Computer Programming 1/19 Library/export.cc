#include <iostream>
#include <vector>
#include "library.h"

using namespace std;

int main(int argc, char* argv[]) {
    vector<book*> library;
    addBook(library, createBook("Harry Potter", "Rowling"));
    addBook(library, createBook("Lord rings", "Tolkien"));
    addBook(library, createBook("Ciao", "Test"));
    addBorrower(getBookByTitle(library, "Harry Potter"), "Salvatore", "10/10/2022");
    addBorrower(getBookByTitle(library, "Harry Potter"), "Test", "12/10/2022");
    addBorrower(getBookByTitle(library, "Ciao"), "hello", "09/10/2022");
    exportBooks(library, argv[1]);
    printBooks(library, true);
}