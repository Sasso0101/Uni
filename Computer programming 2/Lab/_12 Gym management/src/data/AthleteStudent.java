package data;

import java.time.LocalDate;

public class AthleteStudent extends Student implements IAthlete {
    private Relevance relevance;
    private String discipline;

    public AthleteStudent(String name, String surname, String fiscalCode, LocalDate birthDate, String uniName, Degree degree, Relevance relevance, String discipline) {
        super(name, surname, fiscalCode, birthDate, uniName, degree);
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
        double studentTariff = super.getTariff();
        double athleteTariff;
        if (relevance == Relevance.international) athleteTariff = 1000*0.5;
        else athleteTariff = 1000*0.7;
        return Math.min(studentTariff, athleteTariff);
    }

    public String toString() {
        return super.toString()+String.format(" %1$s %2$s %3$s %4$s", uniName, degree, discipline, relevance);
    }
}
