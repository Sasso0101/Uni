public class Card {
    private Seed seed;
    private int value;

    public Card(Seed seed, int value) {
        this.seed = seed;
        if (value < 1 || value > 10) throw new IllegalArgumentException("A card value must be between 1 and 10");
        this.value = value;
    }

    public boolean equals(Object other){
        return (other instanceof Card && seed.equals(((Card)other).seed) && value==((Card)other).value);
    }

    public String toString() {
        return "{" + seed + ", " + value + "}";
    }
}
