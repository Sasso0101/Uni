package bikes;
public class Bike implements Comparable<Bike> {
    protected String manufacturer;

    static public enum bikeType {
        MTB,
        ROAD,
        CITYBIKE,
        UNKNOWN,
        EBIKE,
    }
    protected bikeType type;
    protected double cost;
    protected double wheelSize;
    protected String model;

    public Bike(){
        this.cost=-1;
        this.type= bikeType.UNKNOWN;
        this.wheelSize=0;
        this.manufacturer="NA";
        this.model="NA";
    }

    public Bike(String m,String model, double ws, double c, bikeType t){
        this.cost=c;
        this.type= t;
        this.wheelSize=ws;
        this.manufacturer=m;
        this.model=model;
        if(t == bikeType.EBIKE){
            this.cost=-1;
            this.type= bikeType.UNKNOWN;
            this.wheelSize=0;
            this.manufacturer="Shouldnotarrive here";
            this.model="shoudl not be there";
        }
    }
    public void setType(bikeType type) {
        this.type = type;
    }
    public bikeType getType() {
        return type;
    }
    public void setModel(String model) {
        this.model = model;
    }
    public String getModel() {
        return model;
    }
    public void setManufacturer(String manufacturer) {
        this.manufacturer = manufacturer;
    }
    public String getManufacturer() {
        return manufacturer;
    }
    public String toString(){
        return String.format("Bike: %15s Model %10s Type: %10s Cost: %7.2f WheelSize: %4.1f",
                this.manufacturer, this.getModel(),this.type.toString(), this.cost,this.wheelSize);
    }
    
    @Override
    public int compareTo(Bike o) {
        return this.getManufacturer().compareTo(o.getManufacturer());
    }

    public static void main(String[] argv){
        Bike [] catalog = {
            new Bike("Pinarello","scorpiom",28,4500,bikeType.ROAD),
            new Bike("Coppi","dragon",29,6500,bikeType.MTB),
            new Bike("Campagnolo","Marilu",26,500,bikeType.CITYBIKE),
            new Bike("Campagnolo","goldfinger",26,9500,bikeType.MTB),
        };
        System.out.println(catalog[2].compareTo(catalog[1]));
    }
}
