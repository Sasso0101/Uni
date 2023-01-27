using namespace std;
#include <iostream>

int size = 100;

//int x;
static int x; //not visible, also with "extern" is not visible

int print_hello(int x) {
  cout << "Hello world! x=" << x << endl;
  return ++x;
}

void print_hello() {
  cout << "Hello\n";
}
