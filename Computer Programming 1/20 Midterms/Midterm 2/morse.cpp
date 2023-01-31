#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

struct letter {
    string morse;
    char ascii;
};

struct morseList {
    letter* morse;
    morseList* next;
};

struct wordList {
    string word;
    wordList* next;
};

morseList* addToMorseList(morseList *list, string morse, char ascii) {
    letter *let = new letter;
    let->morse = morse;
    let->ascii = ascii;
    morseList *newElement = new morseList;
    newElement->morse = let;
    if (list != NULL) newElement->next = list;
    return newElement;
}

wordList* addToWordList(wordList* list, string word) {
    wordList *newWord = new wordList;
    newWord->word = word;
    if (list != NULL) newWord->next = list;
    return newWord;
}

wordList* findInWordList(wordList *wordList, string value) {
    while(wordList != NULL) {
        if (wordList->word == value) return wordList;
        wordList = wordList->next;
    }
    return NULL;
}

char findLetter(morseList *morseList, string value) {
    while(morseList != NULL) {
        if (morseList->morse->morse == value) return morseList->morse->ascii;
        morseList = morseList->next;
    }
    return '\0';
}

morseList* importMorseList(char filename[]) {
    fstream input;
    input.open(filename, ios::in);
    char line[10];
    morseList *list = NULL;
    while(!input.eof()) {
        char letter;
        string morse;
        input >> letter;
        input >> morse;
        list = addToMorseList(list, morse, letter);
    }
    input.close();
    return list;
}

void checkWord(wordList **wList, int &uniqueWords, int &totalWords, string word) {
    totalWords++;
    if (findInWordList(*wList, word) == NULL) {
        *wList = addToWordList(*wList, word);
        uniqueWords++;
    }
}

string translateMorse(morseList *list, char filename[], int &uniqueWords, int &totalWords) {
    fstream input;
    input.open(filename, ios::in);
    char line[10];
    string word = "";
    string message = "";
    wordList *wList = NULL;
    while(!input.eof()) {
        string morseLetter;
        input >> morseLetter;
        char asciiLetter = findLetter(list, morseLetter);
        if (morseLetter == "/" || morseLetter.length() == 0) {
            checkWord(&wList, uniqueWords, totalWords, word);
            message += " ";
            word = "";
        }
        else {
            // Check if letter is a character or number
            if (asciiLetter >= 65 && asciiLetter <= 90 || asciiLetter >= 48 && asciiLetter <= 57) word += asciiLetter;
            message += asciiLetter;
        }
    }
    input.close();
    return message;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        cout << "Command syntax is not correct!";
        return 0;
    }
    morseList *list = importMorseList(argv[1]);
    int uniqueWords = 0;
    int totalWords = 0;
    string message = translateMorse(list, argv[2], uniqueWords, totalWords);
    cout << uniqueWords << " / " << totalWords << endl;
    cout << message;
}