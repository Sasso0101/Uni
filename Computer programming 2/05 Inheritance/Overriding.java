class Overriding {
    // public int func() {return 1;}
    public void func() {}
    public int func(int a) {return 1;}
    // void f(int a) {}

    public static void main(String args[]) {
        Overriding ov = new Overriding();
        ov.func();
        ov.func(3);
    }
}