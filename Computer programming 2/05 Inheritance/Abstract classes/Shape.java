abstract class Shape {
    static int a;
    Shape(int a) {
        this.a = a;
    }
    abstract void show();

    public static void main(String args[]) {
        // Shape s = new Shape(5);
        Circle c = new Circle(5);
    }
}

class Circle extends Shape {
    Circle(int a) {
        super(a);
    }
    void show() {
    }
}