import data.BikeCatalog;

public class BikeTest {
    public static void main(String args[]) {
        BikeCatalog c = new BikeCatalog("bikes.txt");
        System.out.println(c);
    }
}
