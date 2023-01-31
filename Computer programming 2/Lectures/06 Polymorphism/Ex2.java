class A {
    private int val;

    public A(int v) {
        val = v;
    }

    public int value() {
        return val;
    }

    public int sum(A o) {
        return value() + o.val;
    }

    public static void main(String[] args) {
        A a1, a2;
        // B b;
        a1 = new A(4);
        a2 = new A(5);
        System.out.println(a1.sum(a2));
        // b = new B(6);
        // System.out.println(a1.sum(a2));
        // System.out.println(a2.sum(b));
        // System.out.println(b.sum(a1));
        // System.out.println(b.sum(b));
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