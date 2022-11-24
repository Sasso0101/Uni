#include <iostream>
#include <fstream>

using namespace std;

char letterAverage[26] = {'e', 't', 'a', 'o', 'i', 'n', 's', 'h', 'r', 'd', 'l', 'c', 'u', 'm', 'w', 'f', 'g', 'y', 'p', 'b', 'v', 'k', 'j', 'x', 'q', 'z'};

void countLetters(int lettersCount[26][2]);
void printLetters(int lettersCount[26][2]);
void matchLetters(int lettersCount[26][2]);

void countLetters(int lettersCount[26][2]) {
    fstream file;
    char c;

    file.open("input.txt");

    while (file >> noskipws >> c) {
        c = tolower(c);
        if (c >= 97 && c <= 122) lettersCount[c-97][0]++;
    }
    file.close();
}

void printLetters(int lettersCount[26][2]) {
    for (int i = 0; i < 26; i++) {
        cout << (char)(i+97) << " " << lettersCount[i][0] << " " << (char)(lettersCount[i][1]) << endl;
    }
}

void matchLetters(int lettersCount[26][2]) {
    int l = 0;
    for (int l = 0; l < 26; l++) {
        int n = 0;
        int max = 0;
        for (int i = 0; i < 26; i++) {
            if (lettersCount[i][0] >= max && lettersCount[i][1] == 0) {
                n = i; max = lettersCount[i][0];
            }
        }
        lettersCount[n][1] = letterAverage[l];
    }
}

void substituteLetters(int lettersCount[26][2]) {
    fstream file;
    char c;
    file.open("input.txt");

    while (!file.eof()) {
        c = file.get();
        c = tolower(c);
        if (c >= 97 && c <= 122) {
            file.seekp((file.tellp() - static_cast<std::streampos>(1)));
            file.put(lettersCount[(c-97)][1]);
            file.seekp(file.tellp());
        }
    }
    file.close();
}

void substituteLetter(char letter, char newLetter) {
    fstream file;
    char c;
    file.open("input.txt");

    while (!file.eof()) {
        c = file.get();
        c = tolower(c);
        if (c == letter) {
            file.seekp((file.tellp() - static_cast<std::streampos>(1)));
            file.put(newLetter);
            file.seekp(file.tellp());
        } else if (c == newLetter) {
            file.seekp((file.tellp() - static_cast<std::streampos>(1)));
            file.put(letter);
            file.seekp(file.tellp());
        }
    }
    file.close();
}

int main() {
    int lettersCount[26][2] = {};
    countLetters(lettersCount);
    matchLetters(lettersCount);
    printLetters(lettersCount);
    substituteLetters(lettersCount);
    while(true) {
        char c, cnew;
        cout << "Enter letter to substitute: ";
        cin >> c;
        cout << "Enter new letter: ";
        cin >> cnew;
        substituteLetter(c, cnew);
    }
}