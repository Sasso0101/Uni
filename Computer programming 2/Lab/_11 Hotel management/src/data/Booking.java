package data;

import java.util.Comparator;
import java.util.LinkedList;

public class Booking {
    private String surname;
    private int numPeople;
    private int numNights;
    private static final int baseNightPrice = 50;
    private LinkedList<Extra> extras = new LinkedList<>();
    public static Comparator<Booking> surnameComparator = new Comparator<Booking>() {
        @Override
        public int compare(Booking o1, Booking o2) {
            return o1.getSurname().compareTo(o2.getSurname());
        }
    };
    public static Comparator<Booking> numPeopleComparator = new Comparator<Booking>() {
        @Override
        public int compare(Booking o1, Booking o2) {
            return Integer.compare(o1.getNumPeople(), o2.getNumPeople());
        }
    };

    public String getSurname() {
        return surname;
    }

    public int getNumPeople() {
        return numPeople;
    }

    public void addExtra(Extra e) {
        extras.add(e);
    }

    public Booking(String surname, int numPeople, int numNights) {
        this.surname = surname;
        this.numPeople = numPeople;
        this.numNights = numNights;
    }

    public String toString() {
        return String.format("Name: %10s\nPeople: %3d\nNights:%3d", surname, numPeople, numNights);
    }
    public String rentListToString() {
        String text = "";
        for (Extra e: extras) if (e instanceof Rent) text += e.toString() + "\n";
        return text;
    }
    public String serviceListToString() {
        String text = "";
        for (Extra e: extras) if (e instanceof Service) text += e.toString() + "\n";
        return text;
    }
    public int getBasePrice() {
        return baseNightPrice*numNights*numPeople;
    }
    public double getFinalPrice() {
        double extrasFinalPrice = 0;
        for (Extra e: extras) {
            extrasFinalPrice += e.getFinalPrice();
        }
        return getBasePrice()+extrasFinalPrice;
    }

    public double getDiscount() {
        double extrasDiscount = 0;
        for (Extra e: extras) {
            extrasDiscount += e.getDiscount();
        }
        return extrasDiscount;
    }
}
