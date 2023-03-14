package data;

public class VeganPizza extends Pizza {
    public VeganPizza(String name, int price) {
        super(name, price);
    }

    public String toString() {
        return "(V) " + super.toString();
    }
}
