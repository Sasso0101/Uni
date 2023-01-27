class Point3D extends Point {
    private double z;

    void move(double x, double y, double z) {
        super.move(x, y);
        this.z = z;
    }

    public String toString() {
        return "(" + x + ", " + y + ", " + z + ")";
    }
}