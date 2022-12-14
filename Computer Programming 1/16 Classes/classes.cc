#include <iostream>

using namespace std;

class Test {
    public:
        void ciao();
};

class Point {
    private:
        int x;
        int y;
    public:
        static int var;
        Point(int x, int y) {
            this->x = x;
            this->y = y;
            var++;
        }
        Point() {
            this->x = 0;
            this->y = 0;
        }
        Point(const Point& p) {
            this->x = p.x + 10;
            this->y = p.y + 10;
        }
        void setCoords(int x, int y) {
            this->x = x;
            this->y = y;
        }
        void print() const {
            cout << x << " " << y;
        }
};

int Point::var = 0;

int main() {
    Point p(5, 10);
    cout << Point::var;
}