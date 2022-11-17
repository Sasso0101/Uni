#include <iostream>
#include <cmath>

using namespace std;

struct point {
    float x;
    float y;
    float z;
    point *next;
};

void printPoint(point *p) {
    cout << "--- x: " << p->x << " y: " << p->y << " z: " << p->z << " ---" << endl;
}

point* createPolygon() {
    int n;
    cout << "Insert number of vertexes: ";
    cin >> n;
    point *firstPoint = NULL;
    point *oldPoint = NULL;
    for (int i = 0; i < n; i++) {
        point *p = new point;
        cout << "Vertex " << i << " coordinates: ";
        cin >> p->x >> p->y >> p->z;
        // If this is the first point update pointer to first point
        if (firstPoint == NULL) firstPoint = p;
        // Otherwise connect previous point to this
        else oldPoint->next = p;
        oldPoint = p;
    }
    // Connect last point to first point
    oldPoint->next = firstPoint;
    return firstPoint;
}

float distance(point *a, point *b) {
    return sqrt(
        pow((a->x) - (b->x), 2) +
        pow((a->y) - (b->y), 2) +
        pow((a->z) - (b->z), 2)
    );
}

float perimeter(point *a) {
    point *p = a;
    float sum = 0;
    do {
        sum += distance(p, p->next);
        p = p->next;
    } while(p != a);
    return sum;
}

int main() {
    // point *a = new point;
    // point *b = new point;
    // point *c = new point;
    // a->x = 0; a->y = 0; a->z = 0;
    // b->x = 1; b->y = 0; b->z = 0;
    // c->x = 1; c->y = 0; c->z = 1;
    // a->next = b; b->next = c; c->next = a;
    // cout << "Distance: " << distance(a, b) << endl;
    // cout << "Perimeter: " << perimeter(a) << endl;
    point *a = createPolygon();
    cout << "Perimeter: " << perimeter(a);
}