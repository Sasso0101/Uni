package data.rent;

import java.time.LocalDateTime;

public class Rent {
    private int code;
    private LocalDateTime datetime;
    private char classCode;
    private String surname;
    private String name;

    public Rent(int code, LocalDateTime datetime, char classCode, String surname, String name) {
        this.code = code;
        this.datetime = datetime;
        this.classCode = classCode;
        this.surname = surname;
        this.name = name;
    }
    public String getSurname() {
        return surname;
    }
    public String getName() {
        return name;
    }
    public LocalDateTime getDateTime() {
        return datetime;
    }
    public int getCode() {
        return code;
    }
    public char getClassCode() {
        return classCode;
    }

    public String toString() {
        return String.format("%1$4d %2$tF %2$tR %3$c %4$22s %5$10s", code, datetime, classCode, surname, name);
    }
}
