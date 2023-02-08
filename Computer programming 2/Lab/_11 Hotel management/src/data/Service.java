package data;

public class Service extends Extra {
    private String variant;

    public Service(String name, String variant, int price, int quantity) {
        super(name, price, quantity);
        this.variant = variant;
    }

    public double getDiscount() {
        double discount = 0;
        if (quantity >= 3) discount = (double) (getFinalPrice()*0.2);
        return discount;
    }
    public double getFinalPrice() {
        return price*quantity;
    }

    public String toString() {
        return String.format("%dx %s %s (%d€): %.2f€", quantity, name, variant, price, getFinalPrice());
    }
}
