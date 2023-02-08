package data;

import java.time.LocalDate;


public class Student extends Subscriber {
    protected String uniName;
    protected Degree degree;

    public Student(String name, String surname, String fiscalCode, LocalDate birthDate, String uniName, Degree degree) {
        super(name, surname, fiscalCode, birthDate);
        this.uniName = uniName;
        this.degree = degree;
    }

    public double getTariff() {
        if (degree == Degree.bachelors) return 1000*0.6;
        return 1000*0.8;
    }

    public String toString() {
        return super.toString()+String.format(" %1$s %2$s", uniName, degree);
    }
}
