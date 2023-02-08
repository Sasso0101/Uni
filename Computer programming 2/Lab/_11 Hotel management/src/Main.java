import data.Booking;
import data.Rent;
import data.Service;
import javafx.application.Application;
import javafx.geometry.Insets;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.scene.text.Text;
import javafx.stage.Stage;

import java.util.ArrayList;
import java.util.Collections;
import java.util.LinkedList;

enum Sort {
    byName,
    byPerson
}
enum Extra {
    services,
    rents
}

public class Main extends Application {
    static LinkedList<Booking> bookings = new LinkedList<>();
    Text bookingText = new Text();
    Text extraText = new Text();
    Text priceText = new Text();

    Button byName = new Button("By name");
    Button byPerson = new Button("By person");
    Button services = new Button("Services");
    Button rents = new Button("Rent");
    Button next = new Button(">");
    Button previous = new Button("<");
    Extra extraState = Extra.services;
    int bookingIndex = 0;

    @Override
    public void start(Stage primaryStage) {
        byName.setOnAction(e -> sortBookings(Sort.byName));
        byPerson.setOnAction(e -> sortBookings(Sort.byPerson));
        priceText.minHeight(100);
        extraText.minHeight(100);
        HBox orderBox = new HBox(byName, byPerson);
        services.setDisable(true);
        services.setOnAction(e -> {
            extraState = Extra.services;
            services.setDisable(true);
            rents.setDisable(false);
            printExtra();
        });
        rents.setOnAction(e -> {
            extraState = Extra.rents;
            services.setDisable(false);
            rents.setDisable(true);
            printExtra();
        });
        HBox extraBox = new HBox(services, rents);
        previous.setDisable(true);
        previous.setOnAction(e -> {
            bookingIndex--;
            printBooking();
        });
        next.setOnAction(e -> {
            bookingIndex++;
            printBooking();
        });
        HBox bottomBox = new HBox(priceText, previous, next);
        VBox windowBox = new VBox(orderBox, bookingText, extraBox, extraText, bottomBox);
        Scene root = new Scene(windowBox);
        windowBox.setSpacing(10);
        windowBox.setPadding(new Insets(10));
        sortBookings(Sort.byName);
        primaryStage.setScene(root);
        primaryStage.setMinWidth(300);
        primaryStage.setMinHeight(360);
        primaryStage.setTitle("Gestione hotel");
        primaryStage.show();
    }

    void sortBookings(Sort order) {
        if (order == Sort.byName) {
            Collections.sort(bookings, Booking.surnameComparator);
            byPerson.setDisable(false);
            byName.setDisable(true);
        } else {
            Collections.sort(bookings, Booking.numPeopleComparator);
            byPerson.setDisable(true);
            byName.setDisable(false);
        }
        bookingIndex = 0;
        printBooking();
    }

    void printBooking() {
        if (bookingIndex == 0) previous.setDisable(true);
        if (bookingIndex > 0) previous.setDisable(false);
        if (bookingIndex == bookings.size()-1) next.setDisable(true);
        if (bookingIndex < bookings.size()-1) next.setDisable(false);
        if (bookings.size() == 0) return;
        Booking b = bookings.get(bookingIndex);
        bookingText.setText(String.format("%s\n\nBase price: %.2f€", b.toString(), (float)b.getBasePrice()));
        printExtra();
        printPrice();
    }

    void printExtra() {
        String text = "";
        if (extraState == Extra.services) {
            text += bookings.get(bookingIndex).serviceListToString();
        } else {
            text += bookings.get(bookingIndex).rentListToString();
        }
        extraText.setText(text);
    }

    void printPrice() {
        String text = "";
        double finalPrice = bookings.get(bookingIndex).getFinalPrice();
        double discount = bookings.get(bookingIndex).getDiscount();
        text = String.format("Total price with extras: %.2f€", finalPrice);
        if (discount > 0) text += String.format("\nDiscount: %.2f€", discount);
        else text += "\nDiscount: -";
        text += String.format("\nTo pay: %.2f€", finalPrice-discount);
        priceText.setText(text);
    }

    public static void main(String[] args) {
        Booking b1 = new Booking("Rossi", 2, 1);
        b1.addExtra(new Service("Skipass", "daily", 45, 2));
        b1.addExtra(new Rent("Snowboard", 130, 25, 1));
        b1.addExtra(new Rent("Snowboard", 150, 25, 1));

        Booking b2 = new Booking("Bianchi", 4, 3);
        b2.addExtra(new Service("Skipass", "half", 80, 2));
        b2.addExtra(new Service("Spa", "sauna", 80, 2));
        b2.addExtra(new Service("Shops", "", 25, 2));
        b2.addExtra(new Rent("Completo sci", 180, 25, 2));
        b2.addExtra(new Rent("Completo sci", 100, 18, 2));

        Booking b3 = new Booking("Verdi", 3, 7);
        b3.addExtra(new Service("Spa", "relax", 250, 3));
        b3.addExtra(new Rent("Ciaspole", 38, 10, 6));
        b3.addExtra(new Rent("Ciaspole", 40, 10, 6));
        b3.addExtra(new Rent("Ciaspole", 37, 10, 6));
        bookings.add(b1);
        bookings.add(b2);
        bookings.add(b3);
        launch(args);
    }
}