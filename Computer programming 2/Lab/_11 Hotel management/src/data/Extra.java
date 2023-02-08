package data;

public abstract class Extra {
    protected String name;
    protected int price;
    protected int quantity;

    Extra(String name, int price, int quantity) {
        this.name = name;
        this.price = price;
        this.quantity = quantity;
    }

    public double getFinalPrice() {
        return price*quantity;
    }

    abstract public double getDiscount();
}
