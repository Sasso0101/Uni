package data;

import java.time.LocalDate;

public class Athlete extends Subscriber implements IAthlete {
    private Relevance relevance;
    private String discipline;

    public Athlete(String name, String surname, String fiscalCode, LocalDate birthDate, Relevance relevance, String discipline) {
        super(name, surname, fiscalCode, birthDate);
        this.relevance = relevance;
        this.discipline = discipline;
    }
    public Relevance getRelevance() {
        return relevance;
    }
    public String getDiscipline() {
        return discipline;
    }

    public double getTariff() {
        if (relevance == Relevance.international) return 1000*0.5;
        return 1000*0.7;
    }

    public String toString() {
        return super.toString()+String.format(" %1$s %2$s", discipline, relevance);
    }
}
