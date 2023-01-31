#include <iostream>

using namespace std;

double sum(int limit, int i = 1) {
    if (i > limit) {
        return 0;
    }
    else {
        return (1/double(i*i))+sum(limit, i+1);
    }
}

int main() {
    int limit = 0;
    cout << "Enter upper limit: ";
    while (!(cin >> limit)) {
        cin.clear();
        cin.ignore(); 
        cout << "You entered an invalid value! Try again." << endl;
    }
    cout << sum(limit) << endl;
}