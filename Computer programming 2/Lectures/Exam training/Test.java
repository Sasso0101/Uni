public class Test {
    public static void main(String[] args) {
        A a = new A();
        System.out.println(a.m(a));
    }
}

interface I {
    public int m(I obj);
}

interface I2 extends I {
    public int m(I obj);
}

class A implements I, I2 {
    public int m(I obj) {
        return 0;
    }
}