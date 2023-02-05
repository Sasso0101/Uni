public class C extends B {
    public C(int x, int y) {
        super(x, y);
        System.out.println("C");
    }
    public static void main(String args[]) {
        C c = new C(1, 2);
        c.ciao();
    }
}
