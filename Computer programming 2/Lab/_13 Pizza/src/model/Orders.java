package model;

import data.AddOnIngredient;
import data.Order;
import data.Pizza;

import java.time.LocalDateTime;
import java.util.LinkedList;

public class Orders {
    LinkedList<Order> orders = new LinkedList<>();

    public Order createAndAddOrder(Pizza p, AddOnIngredient addon) {
        Order o = new Order(p, addon, LocalDateTime.now());
        orders.add(o);
        return o;
    }

    public String toString() {
        String out = "";
        for (Order o: orders) {
            out += o.toString() + "\n";
        }
        return out;
    }
}
