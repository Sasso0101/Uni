package data;

import java.time.LocalDateTime;

public class Order {
    Pizza pizza;
    AddOnIngredient addon;
    LocalDateTime dt;
    int price;

    public Order(Pizza pizza, AddOnIngredient addon, LocalDateTime dt) {
        this.pizza = pizza;
        this.addon = addon;
        this.dt = dt;
        this.price = computePrice();
    }

    private int computePrice() {
        if (addon != null) return pizza.getPrice() + addon.getPrice();
        else return pizza.getPrice();
    }

    public Pizza getPizza() {
        return pizza;
    }

    public AddOnIngredient getAddon() {
        return addon;
    }

    public LocalDateTime getDateTime() {
        return dt;
    }

    public int getPrice() {
        return price;
    }

    public String toString() {
        if (addon != null) return String.format("%3$td/%3$tm/%3$ty %3$tR %1$s (+ %2$s) - %4$d DKK", pizza.getName(), addon.getName(), dt, price);
        else return String.format("%2$td/%2$tm/%2$ty %2$tR %1$s - %3$d DKK", pizza.getName(), dt, price);
    }
}
