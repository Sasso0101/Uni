class Point {
    protected double x;
    protected double y;

    protected void move(double x, double y) {
        this.x = x;
        this.y = y;
    }

    public String toString() {
        return "(" + x + ", " + y + ")";
    }

    public static void main(String args[]) {
        Point p = new Point();
        p.move(3, 4);
        System.out.println(p);
        Point3D p1 = new Point3D();
        p1.move(4, 5, 6);
        System.out.println(p1);
    }
}