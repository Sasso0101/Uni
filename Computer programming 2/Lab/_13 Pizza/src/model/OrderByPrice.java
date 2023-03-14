package model;

import data.Pizza;

import java.util.Comparator;

public class OrderByPrice implements Comparator<Pizza> {
    @Override
    public int compare(Pizza o1, Pizza o2) {
        return Integer.compare(o1.getPrice(), o2.getPrice());
    }
}