package p1;

public class A {
    protected int a;
    public static void main(String args[]) {
        B b = new B();
    }
}
class B extends A {
    B() {
        A a = new A();
        System.out.println(a.a);
    }
}