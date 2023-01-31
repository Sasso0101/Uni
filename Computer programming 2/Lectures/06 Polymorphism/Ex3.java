class A {
    public static void main(String args[]) {
        A a1;
        B b1 = new C();
        a1 = b1;
        a1.f1();
        ((C) a1).f3();
    }
    void f1() {
        System.out.println("A");
    }
}

class B extends A {
    void f1() {
        System.out.println("B");
    };
    void f2() {
        System.out.println("B");
    };
}
class C extends B {
    void f1() {
        System.out.println("C");
    };
    void f2() {
        System.out.println("C");
    };
    void f3() {
        System.out.println("C");
    };
}