package data;

import java.time.Duration;
import java.time.LocalDateTime;

public class Snack extends Product {
    LocalDateTime expiration;
    public Snack(int ID, String description, float cost, int vat, int amount, LocalDateTime expiration) {
        super(ID, description, cost, vat, amount);
        this.expiration = expiration;
    }
    public int isDispensable(int temperature) {
        if (LocalDateTime.now().compareTo(expiration) > 0) return -1;
        return 0;
    }

    public LocalDateTime getExpDate() {
        return expiration;
    }

    public String toString() {
        return super.toString() + String.format(" %1$tD %1$tR", expiration);
    }
}
