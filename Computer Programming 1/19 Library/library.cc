#include "library.h"
#include <iostream>
#include <fstream>

using namespace std;

fstream openFile(const char filename[]) {
    fstream file;
    file.open(filename, ios::in | ios::out);
    if (file.fail()) cout << "Error opening file!" << endl;
    return file;
}

book* createBook(string title, string author) {
    book *newBook = new book;
    newBook->title = title;
    newBook->author = author;
    return newBook;
}

void addBook(vector<book*> &library, book *newBook) {
    library.push_back(newBook);
}

void addBorrower(book *book, string name, string date) {
    borrower *newBorrower = new borrower;
    newBorrower->name = name;
    newBorrower->date = date;
    book->borrowers.push_back(newBorrower);
}

book* getBookByTitle(const vector<book*> &library, string title) {
    for(int i = 0; i < library.size(); i++) {
        if (library[i]->title == title) return library[i];
    }
    return NULL;
}

void printBorrowers(vector<borrower*> &borrowers) {
    if (borrowers.size() == 0) {
        cout << "No borrowers!" << endl;
    } else {
        cout << "Borrowers: ";
        for(int i = 0; i < borrowers.size(); i++) {
            cout << borrowers[i]->name << " (" << borrowers[i]->date << "), ";
        }
    }
}

void printBooks(const vector<book*> &library, bool withBorrowers) {
    for(int i = 0; i < library.size(); i++) {
        cout << "Title: " << library[i]->title << endl;
        cout << "Author: " << library[i]->author << endl;
        if (withBorrowers) printBorrowers(library[i]->borrowers);
        cout << endl;
    }
};

void importBooks(vector<book*> &library, const char filename[]) {
    fstream file = openFile(filename);
    file.ignore(numeric_limits<streamsize>::max(), '\n');
    while(!file.eof()) {
        string row;
        getline(file, row);
        if (row.length() == 0) break;
        int delim = row.find(',');
        string title = row.substr(0, delim);
        string author = row.substr(delim+1, string::npos);
        addBook(library, createBook(title, author));
    }
    file.close();
}

void exportBooks(const vector<book*> &library, const char filename[]) {
    fstream file = openFile(filename);
    file << "title,author" << endl;
    for(int i = 0; i < library.size(); i++) {
        file << library[i]->title << "," << library[i]->author << endl;
    }
    file.close();
}