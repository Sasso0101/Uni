package data.bikes;

public class Bike {
    public enum BikeType {
        mtb,
        road,
        citybike,
        ebike
    }
    private String model;
    private double wheelSize;
    private String manufacturer;
    private BikeType type;
    private double price;

    public Bike(String model, double wheelSize, String manufacturer, BikeType type, double price) {
        this.model = model;
        this.wheelSize = wheelSize;
        this.manufacturer = manufacturer;
        this.type = type;
        this.price = price;
    }

    public String getModel() {
        return model;
    }
    public double getWheelSize() {
        return wheelSize;
    }
    public BikeType BikeType() {
        return type;
    }
    public String getManufacturer() {
        return manufacturer;
    }
    public double getPrice() {
        return price;
    }
    public String toString() {
        return String.format("%8s %30s %10s %.0f %.2f€", manufacturer, model, type, wheelSize, price);
    }
}