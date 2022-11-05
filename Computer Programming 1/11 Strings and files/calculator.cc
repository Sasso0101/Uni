#include <iostream>
#include <cstdlib>

using namespace std;

/* Checks if passed string is a valid integer
    If it is valid saves the value in parameter n
    Returns -1 if string is not a number
    Returns -2 if out of bound
*/
int checkInput(char* input, long int& n) {
    errno = 0;
    char* lastChar;
    n = strtol(input, &lastChar, 10);
    if (*lastChar != '\0') return -1;
    else if (errno == ERANGE) return -2;
    return 0;
}

int main(int argc, char* argv[]) {
    try {
        if (argc != 4) throw ("The input is not valid!");
        long int n1, n2;
        char op;
        float res = 0;
        op = *argv[2];
        if (checkInput(argv[1], n1) != 0) throw ("The input is not valid!");
        if (checkInput(argv[3], n2) != 0) throw ("The input is not valid!");
        switch (op) {
            case '+':
            res = n1 + n2;
            break;
            case '-':
                res = n1 - n2;
                break;
            case 'x':
                res = n1 * n2;
                break;
            case '/':
                if (n2 != 0) res = (float)n1 / (float)n2;
                else throw ("You can't divide by 0!");
                break;
            default:
                throw ("The input is not valid!");
                break;
        }
        cout << n1 << " " << op << " " << n2 << " = " << res << endl;
    } catch (const char* error) {
        cout << error << endl;
    }
    
}