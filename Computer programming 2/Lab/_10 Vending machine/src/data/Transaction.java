package data;

import java.time.LocalDateTime;

public class Transaction {
    private int id;
    private int quantity;
    private float pricePerUnit;
    private float totalPrice;
    LocalDateTime datetime;

    public Transaction(int id, int quantity, float pricePerUnit, float totalPrice, LocalDateTime datetime) {
        this.id = id;
        this.quantity = quantity;
        this.pricePerUnit = pricePerUnit;
        this.totalPrice = totalPrice;
        this.datetime = datetime;
    }

    public LocalDateTime getDateTime() {
        return datetime;
    }
    public float getTotalPrice() {
        return totalPrice;
    }

    public String toString() {
        return String.format("Id: %1$3d Quantity: %2$3d Price: %3$3.2f€ Total price: %4$3.2f€ Date: %5$tD %5$tR", id, quantity, pricePerUnit, totalPrice, datetime);
    }
}
