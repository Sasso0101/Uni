#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;

const char separator    = ' ';

struct node {
    string word;
    int count;
    node *lptr;
    node *rptr;
};

node* addNode(string word, node* leaf);
node* searchWord(string word, node& leaf);
void printWords(node* root, int wordsCount);

node* addNode(string word, node* leaf = NULL) {
    node* newLeaf = new node;
    newLeaf->word = word;
    newLeaf->count = 1;
    newLeaf->lptr = NULL;
    newLeaf->rptr = NULL;
    if (leaf != NULL) {
        if (word.compare(leaf->word) < 0) leaf->lptr = newLeaf;
        else leaf->rptr = newLeaf;
    }
    return newLeaf;
}

node* searchWord(string word, node* root, node* &leaf) {
    leaf = root;
    int comp = word.compare(leaf->word);
    while (
        (comp < 0 && leaf->lptr != NULL) ||
        (comp > 0 && leaf->rptr != NULL)
    ) {
        if (comp < 0) leaf = leaf->lptr;
        else leaf = leaf->rptr;
        comp = word.compare(leaf->word);
    };
    if (comp == 0) return leaf;
    else return NULL;
}

/** If word alredy in tree increment it's counter by one
    Else add word to tree
*/
node* insertOrSearch(string word, node* root = NULL) {
    node *el = NULL;
    if (root == NULL) el = addNode(word);
    else {
        node *leaf = NULL;
        el = searchWord(word, root, leaf);
        if (el != NULL) el->count += 1;
        else el = addNode(word, leaf);
    }
    return el;
}

void printNumber(float n, int width, int precision = 0) {
    cout.precision(precision);
    cout.setf(ios::fixed, ios::floatfield);
    cout << right << setw(width) << setfill(separator) << n;
}

void printWords(node* root, int wordsCount) {
    if (root == NULL) return;
    node *el = root;
    printNumber(el->count, 2);
    cout << " ";
    printNumber((el->count/(float)wordsCount)*100, 5, 2);
    cout << "% ";
    cout << el->word << endl;
    printWords(el->lptr, wordsCount);
    printWords(el->rptr, wordsCount);
};

int main() {
    fstream file;
    string word;
    node *root = NULL;
    int wordsCount = 0;

    file.open("file.txt");
    while (file >> word)
    {
        wordsCount++;
        if (root == NULL) root = insertOrSearch(word);
        else insertOrSearch(word, root);
    }
    printWords(root, wordsCount);
}