package data;

import java.io.*;
import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.Comparator;

public class VendingMachine {
    private int temperature = -18;
    ArrayList<Product> products = new ArrayList<>();
    ArrayList<Transaction> transactions = new ArrayList<>();

    public static Comparator<Product> priceListDescriptionComparator = new Comparator<Product>() {
        @Override
        public int compare(Product p1, Product p2) {
            return p1.getDescription().compareTo(p2.getDescription());
        }
    };
    public static Comparator<Product> priceListPriceComparator = new Comparator<Product>() {
        @Override
        public int compare(Product p1, Product p2) {
            return Float.compare(p2.getPrice(), p1.getPrice());
        }
    };
    public static Comparator<Product> priceListExpDateComparator = new Comparator<Product>() {
        @Override
        public int compare(Product p1, Product p2) {
            LocalDateTime p1ExpDate = null;
            LocalDateTime p2ExpDate = null;
            if (p1 instanceof Frozen) p1ExpDate = ((Frozen) p1).getExpDate();
            else if (p1 instanceof Snack) p1ExpDate = ((Snack) p1).getExpDate();
            if (p2 instanceof Frozen) p2ExpDate = ((Frozen) p2).getExpDate();
            else if (p2 instanceof Snack) p2ExpDate = ((Snack) p2).getExpDate();
            if (p1ExpDate != null && p2ExpDate != null) return p1ExpDate.compareTo(p2ExpDate);
            if (p1ExpDate == null && p2ExpDate == null) return 0;
            if (p1ExpDate != null) return -1;
            return 1;
        }
    };
    public static Comparator<Product> priceListIdComparator = new Comparator<Product>() {
        @Override
        public int compare(Product p1, Product p2) {
            return Integer.compare(p1.getId(), p2.getId());
        }
    };
    public static Comparator<Transaction> transactionsDateComparator = new Comparator<Transaction>() {
        @Override
        public int compare(Transaction t1, Transaction t2) {
            return t1.getDateTime().compareTo(t2.getDateTime());
        }
    };
    public static Comparator<Transaction> transactionsPriceComparator = new Comparator<Transaction>() {
        @Override
        public int compare(Transaction t1, Transaction t2) {
            return Float.compare(t2.getTotalPrice(), t1.getTotalPrice());
        }
    };

    public VendingMachine() {}
    public VendingMachine(String file) {
        importProducts(file);
    }

    public Product getProduct(int id) {
        for (Product p: products) {
            if (p.getId() == id) return p;
        }
        return null;
    }

    public String buyAndGetReceipt(String stringId, String stringQuantity) {
        try {
            int id = Integer.parseInt(stringId);
            int quantity = Integer.parseInt(stringQuantity);
            Product p = getProduct(id);
            if (p == null) return "The entered product does not exist!\n";
            if (quantity <= 0) return "The quantity must be positive!\n";
            return buyAndGetReceipt(p, quantity);
        } catch (NumberFormatException e){
            return "Please enter numeric values only!\n";
        }
    }

    public ArrayList<Product>getProducts() {
        return products;
    }
    public ArrayList<Transaction>getTransactions() {
        return transactions;
    }
    public String buyAndGetReceipt(Product p, int quantity) {
        String message = "";
        if (p.getQuantity() < quantity) message += "There are not enough items in stock\n";
        switch (p.isDispensable(temperature)) {
            case -1 -> message += "Product expired!\n";
            case -2 -> message += "Temperature is too high!\n";
            case -3 -> message += "Product expired (best before date is too far)!\n";
        }
        if (message.length() > 0) return "Product: "+p.getDescription()+"\n"+message;
        float pricePerUnit = p.getPrice()*(1+((float)p.getVat()/100));
        float total = pricePerUnit*quantity;
        p.setQuantity(p.getQuantity()-quantity);
        message = String.format("Quantity delivered: %d\nTotal price: %.2f€\nProduct: %s\nNew quantity: %d", quantity, total, p.getDescription(), p.getQuantity());
        if (p instanceof Frozen) message += String.format("\nRefrigerator temperature: %d°C", temperature);
        transactions.add(new Transaction(p.getId(), quantity, pricePerUnit, total, LocalDateTime.now()));
        return message;
    }

    public void setTemperature(int temperature) {
        this.temperature = temperature;
    }
    public int getTemperature() {
        return temperature;
    }

    public void importProducts(String filename) {
        String path = new File("src/products.txt").getAbsolutePath();
        try (
                FileReader f = new FileReader(path);
                BufferedReader reader = new BufferedReader(f);
                ) {
            String line;
            while ((line = reader.readLine()) != null) {
                if (line.isBlank()) continue;
                String values[] = line.split("\\|");
                for (int i = 0; i < values.length; i++) values[i] = values[i].strip();
                int code = Integer.parseInt(values[0]);
                if (code >= 400) {
                    products.add(new Frozen(code, values[1], Float.parseFloat(values[2]), Integer.parseInt(values[3]), Integer.parseInt(values[4]), LocalDateTime.parse(values[5]), Integer.parseInt(values[6])));
                } else if (code >= 300) {
                    products.add(new Snack(code, values[1], Float.parseFloat(values[2]), Integer.parseInt(values[3]), Integer.parseInt(values[4]), LocalDateTime.parse(values[5])));
                } else if (code >= 200) {
                    products.add(new ColdBeverage(code, values[1], Float.parseFloat(values[2]), Integer.parseInt(values[3]), Integer.parseInt(values[4]), LocalDateTime.parse(values[5])));
                } else if (code >= 100) {
                    products.add(new HotBeverage(code, values[1], Float.parseFloat(values[2]), Integer.parseInt(values[3]), Integer.parseInt(values[4])));
                }
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public String toString() {
        return productsToString();
    }

    public String productsToString() {
        return productsToString(products);
    }

    public static String productsToString(ArrayList<Product> products) {
        String out = "";
        for (Product p: products) {
            out += p.toString()+"\n";
        }
        return out;
    }

    public String transactionsToString() {
        String out = "";
        for (Transaction t: transactions) {
            out += t.toString() + "\n";
        }
        return out;
    }

    public ArrayList<Product> getProductsPresent() {
        ArrayList<Product> productsPresent = new ArrayList<>();
        for (Product p: products) {
            if(p.getQuantity() > 0) productsPresent.add(p);
        }
        return productsPresent;
    }

    public static void main(String args[]) {
        VendingMachine m = new VendingMachine();
        m.importProducts("products.txt");
        System.out.println(m);
    }
}
