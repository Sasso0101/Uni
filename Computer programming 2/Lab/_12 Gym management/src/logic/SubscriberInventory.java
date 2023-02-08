package logic;

import data.*;

import java.io.*;
import java.time.LocalDate;
import java.time.format.DateTimeFormatter;
import java.util.LinkedList;

public class SubscriberInventory {
    private LinkedList<Subscriber> subscribers = new LinkedList<>();

    public LinkedList<Subscriber> getAll() {
        return subscribers;
    }
    public LinkedList<Subscriber> getStudents() {
        LinkedList<Subscriber> out = new LinkedList<>();
        for (Subscriber s: subscribers) {
            if (s instanceof Student) out.add(s);
        }
        return out;
    }

    public LinkedList<Subscriber> getAthletes() {
        LinkedList<Subscriber> out = new LinkedList<>();
        for (Subscriber s: subscribers) {
            if (s instanceof IAthlete) out.add(s);
        }
        return out;
    }
    public static String listToString(LinkedList<Subscriber> subscribers) {
        String text = "";
        for (Subscriber s: subscribers) {
            text += s.toString() + " | Tariff: " + s.getTariff() + "€\n";
        }
        return text;
    }

    public void importInventory(String file) {
        String path = new File("src/"+file).getAbsolutePath();
        try (
                FileReader f = new FileReader(path);
                BufferedReader reader = new BufferedReader(f)
        ) {
            String line;
            while ((line = reader.readLine()) != null) {
                if (line.length() == 0) continue;
                String[] values = line.split(",");
                for (int i = 0; i < values.length; i++) {
                    values[i] = values[i].strip();
                }
                LocalDate date = LocalDate.parse(values[3], DateTimeFormatter.ofPattern("d/M/y"));
                if (values.length == 4) {
                    subscribers.add(new Subscriber(values[1], values[0], values[2], date));
                } else if (values.length == 6) {
                    subscribers.add(new Student(values[1], values[0], values[2], date, values[4], Degree.parse(values[5])));
                } else if (values.length == 8) {
                    if (!values[4].equals(""))
                        subscribers.add(new AthleteStudent(values[1], values[0], values[2], date, values[4], Degree.parse(values[5]), Relevance.parse(values[7]), values[6]));
                    else subscribers.add(new Athlete(values[1], values[0], values[2], date, Relevance.parse(values[7]), values[6]));
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public String toString() {
        return listToString(subscribers);
    }

    public static void main(String[] args) {
        SubscriberInventory s = new SubscriberInventory();
        s.importInventory("data.txt");
        System.out.println(s);
    }
}
