package data;

public class Rent extends Extra {
    private int size;

    public Rent(String name, int size, int price, int days) {
        super(name, price, days);
        this.size = size;
    }

    public double getDiscount() {
        double discount = 0;
        if (quantity > 1) discount = (quantity-1)*0.05*price;
        System.out.println(discount);
        return discount;
    }

    public String toString() {
        return String.format("%s, size %d (%d€ x %d days): %.2f€", name, size, price, quantity, getFinalPrice());
    }
}
