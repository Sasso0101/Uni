#include <iostream>
#include <vector>

using namespace std;

struct borrower {
    string name;
    string date;
};

struct book {
    string author;
    string title;
    vector<borrower*> borrowers;
};

book* createBook(string author, string title) {
    book *newBook = new book;
    newBook->author = author;
    newBook->title = title;
    return newBook;
};

void addBook(vector<book*> &library, book *newBook) {
    library.push_back(newBook);
};

void addBorrower(book *book, string name, string date) {
    borrower *newBorrower = new borrower;
    newBorrower->name = name;
    newBorrower->date = date;
    book->borrowers.push_back(newBorrower);
};

book* getBookByTitle(const vector<book*> &library, string title) {
    for(int i = 0; i < library.size(); i++) {
        if (library[i]->title == title) return library[i];
    }
    return NULL;
};

void printBooks(const vector<book*> &library, bool withBorrowers) {
    for(int i = 0; i < library.size(); i++) {
        cout << "Title: " << library[i]->title << " Author: " << library[i]->author << endl;
        if (withBorrowers) {
            cout << "Borrowers:" << endl;
            for(int j = 0; j < library[i]->borrowers.size(); j++) {
                cout << "Name: " << library[i]->borrowers[j]->name << " Date: " << library[i]->borrowers[j]->date << endl;
            }
        }
    }
};

int main() {
    vector<book*> library;
    addBook(library, createBook("Rowling", "Harry Potter"));
    addBorrower(getBookByTitle(library, "Harry Potter"), "Salvatore", "10/10/2022");
    printBooks(library, true);
}