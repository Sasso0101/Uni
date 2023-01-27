public class A {
    public void print(A p) {
        System.out.println("A");
    }
    public static void main(String args[]) {
        A a1, a2;
        B b1;
        C c1;
        a1 = new B();
        b1 = new B();
        c1 = new C();
        a2 = new C();
        b1.print(b1);
        a1.print(b1);
        b1.print(c1);
        c1.print(c1);
        c1.print(a1);
        a2.print(c1); 
    }
}
class B extends A {
    public void print(B p) {
        System.out.println("B");
    }
    public void print(A p) {
        System.out.println("A-B");
    }
}
class C extends B {
    public void print(C p) {
        System.out.println("C");
    }
    public void print(A p) {
        System.out.println("A-C");
    }
} 