package data;

import java.time.Duration;
import java.time.LocalDateTime;

public class ColdBeverage extends Product {
    LocalDateTime bestBefore;
    public ColdBeverage(int ID, String description, float cost, int vat, int amount, LocalDateTime bestBefore) {
        super(ID, description, cost, vat, amount);
        this.bestBefore = bestBefore;
    }

    public int isDispensable(int temperature) {
        if (Duration.between(LocalDateTime.now(), bestBefore).compareTo(Duration.ofDays(-7)) < 0) return -3;
        return 0;
    }

    public String toString() {
        return super.toString() + String.format("(best before %1$tD %1$tR)", bestBefore);
    }
}
