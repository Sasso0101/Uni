import bikes.*;
import java.util.*;

public class app {
    public static void main(String[] argv){
        List <Bike> catalog = new ArrayList<Bike>();
        catalog.add(new Bike( "Pinarello","scorpiom",28,4500, Bike.bikeType.ROAD));
        catalog.add(new Bike("Coppi","dragon",29,6500, Bike.bikeType.MTB));
        catalog.add(new Bike("Campagnolo","Marilu",26,500, Bike.bikeType.CITYBIKE));
        catalog.add(new Bike("Coppi","dragon2",26,4000, Bike.bikeType.MTB));
        catalog.add(new EBike("FOCUS","wind",29,5500, Bike.bikeType.EBIKE,600));
        catalog.add(new Bike("Campagnolo","Aldebaran",28,500, Bike.bikeType.CITYBIKE));
        // System.out.println("Iteration through the list with Enhanced for");
        // for (Bike bike: catalog) {
        //     System.out.println(bike);
        // }
        Collections.sort(catalog);
        System.out.println("Iteration through iterator");
        Iterator<Bike> i = catalog.iterator();
        while (i.hasNext()) {
            Bike b = i.next();
            System.out.println(b);
        } 
    }
}
