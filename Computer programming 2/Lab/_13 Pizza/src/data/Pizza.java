package data;

import java.util.LinkedList;

public class Pizza {
    LinkedList<Ingredient> ingredients = new LinkedList<>();
    int price;
    String name;

    public Pizza(String name, int price) {
        this.name = name;
        this.price = price;
    }
    public void addIngredient(Ingredient i) {
        ingredients.add(i);
    }

    public int getPrice() {
        return this.price;
    }

    public String getName() {
        return name;
    }

    public String toString() {
        String ingredients = "";
        for (Ingredient i: this.ingredients) {
            ingredients += i.toString() + ", ";
        }
        ingredients = ingredients.substring(0, ingredients.length()-2);
        return String.format("%s - %d DKK (%s)", name, price, ingredients);
    }
}
