import java.util.*;

public class Deck {
    LinkedList<Card> deck;

    protected void generateDeck() {
        for (int i = 1; i < 11; i++) {
            for (Seed seed : Seed.values()) {
                deck.add(new Card(seed, i));
            }
        }
    }

    public String toString() {
        String output = "";
        for (Card card : deck) {
            output += card + ", ";
        }
        return output;
    }

    public void shuffle() {
        Collections.shuffle(deck);
    }

    public int getNumCards() {
        return deck.size();
    }

    public Card drawCard() {
        if(deck.isEmpty()) return null;
        else return deck.removeFirst();
    }

    public Deck() {
        this(1);
    }

    public Deck(int setsOfDecks) {
        deck = new LinkedList<Card>();
        for (int i = 0; i < setsOfDecks; i++) {
            generateDeck();
        }
        shuffle();
    }

    public static void main(String args[]) {
        Deck raminoDeck = new Deck(2);
        ArrayList<Card> draw = new ArrayList<Card>();
        for (int i = 0; i < 10; i++) {
            Card c = raminoDeck.drawCard();
            if (draw.contains(c)) System.out.println("Duplicate! "+c);
            draw.add(c);
        }
        System.out.println(draw);
    }
}
