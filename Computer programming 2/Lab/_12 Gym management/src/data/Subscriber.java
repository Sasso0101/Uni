package data;

import java.time.LocalDate;
import java.util.Comparator;

public class Subscriber {
    protected String name;
    protected String surname;
    protected String fiscalCode;
    protected LocalDate birthDate;

    public Subscriber(String name, String surname, String fiscalCode, LocalDate birthDate) {
        this.name = name;
        this.surname = surname;
        this.fiscalCode = fiscalCode;
        this.birthDate = birthDate;
    }

    public double getTariff() {
        if (birthDate.getYear() < 1968) return 1000*0.65;
        return 1000;
    }

    public String toString() {
        return String.format("%1$s %2$s %3$s %4$tD", surname, name, fiscalCode, birthDate);
    }

    public static Comparator<Subscriber> ageComparator = new Comparator<Subscriber>() {
        @Override
        public int compare(Subscriber o1, Subscriber o2) {
            return o1.birthDate.compareTo(o2.birthDate);
        }
    };
    public static Comparator<Subscriber> nameComparator = new Comparator<Subscriber>() {
        @Override
        public int compare(Subscriber o1, Subscriber o2) {
            int surname = o1.surname.compareTo(o2.surname);
            if (surname == 0) return o1.name.compareTo(o2.name);
            return surname;
        }
    };
}
