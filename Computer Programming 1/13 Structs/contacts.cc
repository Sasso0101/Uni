#include <iostream>
#include <string>
#include <fstream>

using namespace std;

struct contact {
    string name;
    string surname;
    string phone;
    string email;
    contact* next;
};

void printContact(contact *c);
void printContactIndex(contact *c, int i);
void printContacts();
void printContactsIndex();
contact* createContact();
contact* createContact(string name, string surname, string phone, string email);
void appendContact();
void appendContact(contact *contact);
contact* getNthContact(int n);
contact* getNthContact(int n, contact* & previous,  contact* & next);
int contactChoser();
void removeContact();
void removeContact(contact *previous, contact *c, contact *next);
void exportContact();
void exportContact(contact *c);
void menu();

contact* FIRST = NULL;
contact* LAST = NULL;

void printContactIndex(contact *c, int i) {
    cout << i << ": " << c->name << " " << c->surname << endl;
}

void printContactsIndex() {
    if (FIRST == NULL) {
        cout << "No contacts found!" << endl;
        return;
    }
    // Print name and surname of contacts
    contact *c = FIRST;
    int i = 1;
    do {
        printContactIndex(c, i);
        i++;
    } while((c = c->next) != NULL);
}

void printContact(contact *c) {
    cout << "--------------" << endl;
    cout << "Name: " << c->name << endl;
    cout << "Surname: " << c->surname << endl;
    cout << "Phone: " << c->phone << endl;
    cout << "Email: " << c->email << endl;
    cout << "--------------" << endl;
}

void printContacts() {
    if (FIRST == NULL) {
        cout << "No contacts found!" << endl;
        return;
    }
    contact *c = FIRST;
    do {
        printContact(c);
    } while((c = c->next) != NULL);
    return;
}

void appendContact() {
    contact *c = createContact();
    appendContact(c);
    return;
}

void appendContact(contact *c) {
    if (FIRST == NULL) FIRST = c;
    else LAST->next = c;
    LAST = c;
    return;
}

contact* createContact() {
    string name, surname, phone, email;
    cout << "Enter name: ";
    cin >> name;
    cout << "Enter surname: ";
    cin >> surname;
    cout << "Enter phone: ";
    cin >> phone;
    cout << "Enter email: ";
    cin >> email;
    return createContact(name, surname, phone, email);
}

contact* createContact(string name, string surname, string phone, string email) {
    contact* c = new contact;
    c->name = name;
    c->surname = surname;
    c->phone = phone;
    c->email = email;
    c->next = NULL;
    return c;
}

contact* getNthContact(int n) {
    return getNthContact(n, NULL, NULL);
}

// Gets nth element in linked list
// If there are not enough elements returns NULL
contact* getNthContact(int n, contact* & previous,  contact* & next) {
    contact *c = FIRST;
    previous = NULL; next = NULL;
    int i = 1;
    // Loops until last element is reached or index is n
    while((next = c->next) != NULL && i < n) {
        previous = c;
        c = next;
        i++;
    };
    if (i < n) return NULL;
    return c;
}

int contactChoser() {
    printContactsIndex();
    int n = 0;
    cout << "Enter number of contact: ";
    cin >> n;
    return n;
}

void exportContact() {
    int n = contactChoser();
    contact *next, *c, *previous;
    if ((c = getNthContact(n, previous, next)) == NULL) {
        cout << "The contact does not exist!";
        return;
    };
    exportContact(c);
    cout << "Generated file output.vcard!" << endl;
}

void exportContact(contact *c) {
    fstream output;
    output.open("output.vcard", ios::out);
    output << "BEGIN:VCARD" << endl;
    output << "VERSION:4.0" << endl;
    output << "N:" << c->surname << ";" << c->name << endl;
    output << "TEL;TYPE=HOME:" << c->phone << endl;
    output << "EMAIL:" << c->email << endl;
    output << "END:VCARD" << endl;
    output.close();
}

void removeContact() {
    int n = contactChoser();
    contact *next, *c, *previous;
    if ((c = getNthContact(n, previous, next)) == NULL) {
        cout << "The contact does not exist!";
        return;
    };
    removeContact(previous, c, next);
    cout << "Contact deleted!";
}

void removeContact(contact *previous, contact *c, contact *next) {
    if (previous != NULL && next != NULL) {
        // If previous end next element exist just link them together
        previous->next = next;
    } else if (next == NULL) {
        // If next element does not exist update last pointer
        LAST = previous;
        // If previous element exists set next of previous element to null
        if (previous != NULL) previous->next = NULL;
    }
    if (previous == NULL) {
        FIRST = next;
    }
    delete c;
}

void editContact() {
    int n = contactChoser();
    contact *newContact = createContact();
    contact *oldContact = getNthContact(n);
    newContact->next = oldContact->next;
    oldContact = newContact;
}

void menu() {
    int c = 0;
    while (c != 6) {
        cout << endl << "--- OPERATIONS ---" << endl;
        cout << "1) Print contacts" << endl;
        cout << "2) Add contact" << endl;
        cout << "3) Edit contact" << endl;
        cout << "4) Remove contact" << endl;
        cout << "5) Export contact to VCARD" << endl;
        cout << "6) Quit" << endl;
        cout << "Enter operation: ";
        cin >> c;
        switch (c) {
            case 1:
                printContacts();
                break;
            case 2:
                appendContact();
                break;
            case 3:
                editContact();
                break;
            case 4:
                removeContact();
                break;
            case 5:
                exportContact();
                break;
            case 6:
                cout << "Bye!" << endl;
                break;
            default:
                cout << "Invalid operation!" << endl;
                break;
        }
    }
}

int main() {
    menu();
}