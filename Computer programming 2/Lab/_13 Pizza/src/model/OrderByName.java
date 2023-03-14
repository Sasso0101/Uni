package model;

import data.Pizza;

import java.util.Comparator;

public class OrderByName implements Comparator<Pizza> {
    @Override
    public int compare(Pizza o1, Pizza o2) {
        return o1.getName().compareTo(o2.getName());
    }
}
