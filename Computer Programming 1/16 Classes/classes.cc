#include <iostream>

using namespace std;

class Point {
    private:
        int x;
        int y;
    public:
        Point(int x, int y) {
            this->x = x;
            this->y = y;
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

int main() {
    Point p(10, 20);
    Point p1(p);
    p.print();
    p1.print();
}