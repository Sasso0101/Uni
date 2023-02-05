package data;
import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.*;
import data.bikes.*;
import data.bikes.Bike.BikeType;

public class BikeCatalog {
    public ArrayList<Bike> catalog;

    public void importFromCSV(String filename) {
        try (
            FileReader file = new FileReader(filename);
            BufferedReader reader = new BufferedReader(file);
        ) {
            String line;
            while((line = reader.readLine()) != null) {
                try {
                    String entries[] = line.split(",");
                    Bike b = null;
                    double wheelSize = Double.parseDouble(entries[3]);
                    double price = Double.parseDouble(entries[4]);
                    switch(entries[2].toLowerCase().trim()) {
                        case "road":
                            b = new Bike(entries[1].trim(), wheelSize, entries[0].trim(), BikeType.road, price);
                            break;
                        case "city":
                            b = new Bike(entries[1].trim(), wheelSize, entries[0].trim(), BikeType.citybike, price);
                            break;
                        case "mtb":
                            b = new Bike(entries[1].trim(), wheelSize, entries[0].trim(), BikeType.mtb, price);
                            break;
                        case "ebike":
                            double batteryCapacity = Double.parseDouble(entries[5]);
                            b = new EBike(entries[1].trim(), wheelSize, entries[0].trim(), BikeType.ebike, price, batteryCapacity);
                            break;
                    }
                    if (b != null) catalog.add(b);
                } catch (NumberFormatException e) {
                    System.out.println("An error occured while converting the data from the file!");
                    e.printStackTrace();
                }
            }
        } catch (FileNotFoundException e) {
            System.out.println("The file was not found!");
            e.printStackTrace();
        } catch (IOException e) {
            System.out.println("there was an error while reading the file!");
            e.printStackTrace();
        }
    }
    public BikeCatalog() {
        catalog = new ArrayList<Bike>();
    }
    public BikeCatalog(String filename) {
        catalog = new ArrayList<Bike>();
        importFromCSV(filename);
    }
    public void sortByWheelSize() {
        Collections.sort(catalog, new Comparator<Bike>() {
            @Override
            public int compare(Bike b1, Bike b2) {
                int ret = (int) (b1.getWheelSize() - b2.getWheelSize());
                if (ret != 0) return ret;
                return b1.getModel().compareTo(b2.getModel());
            }
        });
    }
    public String toString() {
        String out = "";
        for (Bike bike: catalog) {
            out += bike + "\n";
        }
        return out;
    }
}
