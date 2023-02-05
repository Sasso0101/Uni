package data;

import java.time.Duration;
import java.time.LocalDateTime;

public class Frozen extends Product {
    LocalDateTime expiration;
    int minTemp;

    public Frozen(int ID, String description, float cost, int vat, int amount, LocalDateTime expiration, int maxTemp) {
        super(ID, description, cost, vat, amount);
        this.expiration = expiration;
        this.minTemp = maxTemp;
    }
    public int isDispensable(int temperature) {
        if (Duration.between(LocalDateTime.now(), expiration).toDays() < 0) return -1;
        if (temperature > minTemp) return -2;
        return 0;
    }

    public LocalDateTime getExpDate() {
        return expiration;
    }

    public String toString() {
        return super.toString() + String.format(" %1$tD %1$tR %2$d°C", expiration, minTemp);
    }
}
