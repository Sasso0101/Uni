package data.car;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;

public class CarInventory {
    ArrayList<Car> cars = new ArrayList<Car>();

    public static Comparator<Car> priceComparator = new Comparator<Car>() {
        @Override
        public int compare(Car c1, Car c2) {
            return Float.compare(c1.getPrice(), c2.getPrice());
        }
    };
    public static Comparator<Car> manufacturerComparator = new Comparator<Car>() {
        @Override
        public int compare(Car c1, Car c2) {
            return c1.getManufacturer().compareTo(c2.getManufacturer());
        }
    };
    public static Comparator<Car> gearboxPriceComparator = new Comparator<Car>() {
        @Override
        public int compare(Car c1, Car c2) {
            int result = c1.getGearbox().compareTo(c2.getGearbox());
            if (result == 0) result = Float.compare(c1.getPrice(), c2.getPrice());
            return result;
        }
    };

    public CarInventory() {}

    public CarInventory(String path) {
        importInventory(path);
    }

    public void importInventory(String path) {
        try (
            FileReader file = new FileReader(path);
            BufferedReader reader = new BufferedReader(file);
        ) {
            String line;
            line = reader.readLine(); //Skip first line
            while((line = reader.readLine()) != null) {
                String values[] = line.split(",");
                float price = Float.parseFloat(values[6].trim());
                Car c = new Car(values[0].trim(), values[1].trim(), values[2].trim(), values[3].trim().charAt(0), values[4].trim(), values[5].trim(), price);
                cars.add(c);
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public String toString() {
        String out = "";
        for (Car car: cars) {
            out += car + "\n";
        }
        return out;
    }
    public CarInventory filter(char classCode) {
        CarInventory filteredCars = new CarInventory();
        for (Car car: cars) {
            if (car.getClassCode() == classCode) filteredCars.cars.add(car);
        }
        return filteredCars;
    }
    public void sort(Comparator<Car> comparator) {
        Collections.sort(cars, comparator);
    }
    public float getBasePrice(char classCode) {
        for (Car c: cars) {
            if (c.getClassCode() == classCode) return c.getPrice();
        }
        return 0;
    }

    public static void main(String args[]) {
        CarInventory inventory = new CarInventory();
        inventory.importInventory("pricelist.txt");
        System.out.println(inventory);
    }
}
