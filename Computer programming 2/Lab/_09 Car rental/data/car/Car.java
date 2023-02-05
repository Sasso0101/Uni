package data.car;

public class Car {
    private String classOfCar;
    private String type;
    private String gearbox;
    private char classCode;
    private String manufacturer;
    private String model;
    private float price;

    public Car(String classOfCar, String type, String gearbox, char classCode, String manufacturer, String model, float price) {
        this.classOfCar = classOfCar;
        this.type = type;
        this.gearbox = gearbox;
        this.classCode = classCode;
        this.manufacturer = manufacturer;
        this.model = model;
        this.price = price;
    }
    public char getClassCode() {
        return classCode;
    }
    public float getPrice() {
        return price;
    }
    public String getManufacturer() {
        return manufacturer;
    }
    public String getGearbox() {
        return gearbox;
    }
    public String toString() {
        return String.format("%10s %20s %15s %20s %15s %c %.2f", manufacturer, model, gearbox, classOfCar, type, classCode, price);
    }
}
