#include <iostream>
#include <string>
using namespace std;

class Book{
    private:
        string author;
        string title;
    public:
        Book(string author, string title){
            this->author = author;
            this->title = title;
        }
};

class Borrower{
    private:
        string name;
    public:
        Borrower(string name){this->name = name;}
};

class Borrowing{
};

class Library{
    private:
        struct date{
            int year;
            int month;
            int day;
        };

        template<typename T>
        struct node{
            T value;
            node* next;

						node<T>(T val): value(val) {}
        };

        // node<Book>* list_book = NULL;
        // node<Borrower>* list_borrower = NULL;
        // node<Borrowing>* list_borrowing = NULL;

        template<typename T>
        void p_add_node(node<T>* &list, T A){
            if(list == NULL){
                list = new node<T>(A);
            }
            else{
                node<T>* p = list;
                while(p->next!=NULL){p=p->next;}

                p->next = new node<T>(A);
                p = p->next;
            }
        }
    public:
        Library(){}
        void Lib_add_book(string author, string title,node<Book>* list_book){
            Book A(author,title);
            p_add_node<Book>(list_book,A);
        }
        void Lib_add_borrower(string name, node<Borrower>* list_borrower){
            Borrower A(name);
            p_add_node<Borrower>(list_borrower,A);
        }
};

int main(){


    return 0;
}