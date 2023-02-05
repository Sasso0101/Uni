package data.bikes;

public class EBike extends Bike {
    private double batteryCapacity;

    public EBike(String model, double wheelSize, String manufacturer, BikeType type, double price, double batteryCapacity) {
        super(model, wheelSize, manufacturer, type, price);
        this.batteryCapacity = batteryCapacity;
    }
    public double getBatteryCapacity() {
        return batteryCapacity;
    }
    public String toString() {
        return super.toString()+" "+String.format("%.2f", batteryCapacity);
    }
}
