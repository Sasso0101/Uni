package data;

public class AddOnIngredient extends Ingredient {
    int price;
    public AddOnIngredient(String name, int price) {
        super(name);
        this.price = price;
    }

    public int getPrice() {
        return price;
    }

    public String toString() {
        return super.toString() + " - " + price + " DKK";
    }
}
