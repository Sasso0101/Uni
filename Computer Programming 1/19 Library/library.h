#ifndef LIBRARY_H
#define LIBRARY_H

#include <string>
#include <vector>

using namespace std;

struct borrower {
    string name;
    string date;
};

struct book {
    string title;
    string author;
    vector<borrower*> borrowers;
};

book* createBook(string title, string author);
void addBook(vector<book*> &library, book *newBook);
void addBorrower(book *book, string name, string date);
book* getBookByTitle(const vector<book*> &library, string title);

void printBorrowers(vector<borrower*> &borrowers);
void printBooks(const vector<book*> &library, bool withBorrowers = true);

void importBooks(vector<book*> &library, const char filename[]);
void exportBooks(const vector<book*> &library, const char filename[]);

#endif