package data;

public class Ingredient {
    String name;
    public Ingredient(String name) {
        this.name = name;
    }

    public String getName() {
        return name;
    }

    public String toString() {
        return name;
    }

    public boolean equals(Ingredient i) {
        return this.name.equals(i.getName());
    }
}
