package data.rent;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.time.Duration;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;

import data.car.CarInventory;

public class RentInventory {
    ArrayList<Rent> rents = new ArrayList<Rent>();

    public static Comparator<Rent> nameComparator = new Comparator<Rent>() {
        @Override
        public int compare(Rent r1, Rent r2) {
            int result = r1.getSurname().compareTo(r2.getSurname());
            if (result == 0) result = r1.getName().compareTo(r2.getName());
            return result;
        }
    };
    public static Comparator<Rent> dateComparatorAsc = new Comparator<Rent>() {
        @Override
        public int compare(Rent r1, Rent r2) {
            return r1.getDateTime().compareTo(r2.getDateTime());
        }
    };
    public static Comparator<Rent> dateComparatorDesc = new Comparator<Rent>() {
        @Override
        public int compare(Rent r1, Rent r2) {
            return r2.getDateTime().compareTo(r1.getDateTime());
        }
    };

    public RentInventory() {}

    public RentInventory(String path) {
        importRents(path);
    }

    public void importRents(String path) {
        try (
            FileReader file = new FileReader(path);
            BufferedReader reader = new BufferedReader(file);
        ) {
            String line;
            while((line = reader.readLine()) != null) {
                String values[] = line.split(",");
                int code = Integer.parseInt(values[0]);
                DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy.MM.dd H.m");
                LocalDateTime datetime = LocalDateTime.parse(values[1], formatter);
                Rent r = new Rent(code, datetime, values[2].trim().charAt(0), values[5].trim(), values[4].trim());
                rents.add(r);
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    public void sort(Comparator<Rent> comparator) {
        Collections.sort(rents, comparator);
    }

    public String toString() {
        String out = "";
        for (Rent rent: rents) {
            out += rent + "\n";
        }
        return out;
    }

    public Rent getRent(int code) {
        for (Rent r: rents) {
            if (r.getCode() == code) return r;
        }
        return null;
    }

    private static int computePeriods(int hours) {
        if (hours % 6 > 0) return (hours/6)+1;
        else return (hours/6);
    }

    public String getReceipt(CarInventory cars, Rent r) {
        if (r == null) return "";
        Duration duration = Duration.between(r.getDateTime(), LocalDateTime.now());
        long seconds = duration.toSeconds();
        int hours = (int)Math.ceil((double)seconds / 3600);
        int days = (int)Math.ceil((double)hours / 24);
        double price = 0;
        double discount = 0;
        char classCode = Character.toUpperCase(r.getClassCode());
        int periods = computePeriods(hours);
        float basePrice = cars.getBasePrice(classCode);
        price = periods*basePrice;
        if (hours > 36) {
            if (classCode == 'D' || classCode == 'E' || classCode == 'F') {
                discount = (double)days*1.15;
            } else {
                discount = days;
            }
            discount = discount > 15 ? 15 : discount;
            price = price * (1-(discount/100));
        }
        return String.format("Number of days: %d\n6h periods: %d\nBase price: %.2f€\nDiscount: %.2f%%\nFinal price: %.2f€\nDriver: %s %s", days, periods, basePrice, discount, price, r.getName(), r.getSurname());
    }

    public void removeRent(Rent r) {
        rents.remove(r);
    }

    public static void main(String args[]) {
        RentInventory inventory = new RentInventory();
        inventory.importRents("persone.txt");
        System.out.println(inventory);
    }
}
