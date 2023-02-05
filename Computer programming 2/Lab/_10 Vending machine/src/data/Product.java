package data;

public class Product {
    private int id;
    private String description;
    private float price;
    private int vat;
    private int quantity;

    public Product(int id, String description, float price, int vat, int quantity) {
        this.id = id;
        this.description = description;
        this.price = price;
        this.vat = vat;
        this.quantity = quantity;
    }

    public int getId() {
        return id;
    }
    public int getQuantity() {
        return quantity;
    }
    public void setQuantity(int quantity) {
        this.quantity = quantity;
    }
    public float getPrice() {
        return price;
    }
    public int getVat() {
        return vat;
    }

    public String getDescription() {
        return description;
    };

    public int isDispensable(int temperature) {
        return 0;
    }

    public String toString() {
        return String.format("%3d %20s %4.2f€ %2d%% %4d", id, description, price, vat, quantity);
    }
}
