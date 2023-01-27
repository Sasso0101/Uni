class A {
    private int val;

    public A(int v) {
        val = v;
    }

    public int value() {
        return val;
    }

    public int sum(A o) {
        return value() + o.value();
    }
}

class B extends A {
    B(int v) {
        super(v);
    }

    public int sum(A o) {
        return value() + o.value() + 2;
    }

    public int sum(B o) {
        return value() + o.value() + 1;
    }
}