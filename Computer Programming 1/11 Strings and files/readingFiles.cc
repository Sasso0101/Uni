#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char* argv[]) {
    try {
        if (argc != 2) throw ("The number of paramenters is wrong!");
        fstream input;
        input.open(argv[1], ios::in);
        if (!input.good()) throw ("The file does not exist or it can't be opened!");
        cout << input.rdbuf();
    } catch (const char* error) {
        cout << error << endl;
    }
}