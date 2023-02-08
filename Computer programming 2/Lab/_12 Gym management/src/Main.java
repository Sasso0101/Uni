import data.Subscriber;
import gui.ConfirmationBox;
import javafx.application.Application;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.scene.text.Text;
import javafx.stage.Stage;
import logic.SubscriberInventory;

import java.util.Collections;
import java.util.LinkedList;

enum Filter {
    All,
    Students,
    Athletes
}
enum Order {
    ByName,
    ByAge
}

public class Main extends Application {
    SubscriberInventory subscribers = new SubscriberInventory();
    Text text = new Text();

    Filter filter = Filter.All;
    Order order = Order.ByName;
    Button filterAll = new Button("All");
    Button filterStudents = new Button("Students");
    Button filterAthletes = new Button("Athletes");
    Button orderByName = new Button("By name");
    Button orderByAge = new Button("By age");

    Stage stage;
    @Override
    public void start(Stage primaryStage) {
        stage = primaryStage;
        subscribers.importInventory("data.txt");
        filterAll.setOnAction(e -> {
            filter = Filter.All;
            update();
        });
        filterAthletes.setOnAction(e -> {
            filter = Filter.Athletes;
            update();
        });
        filterStudents.setOnAction(e -> {
            filter = Filter.Students;
            update();
        });
        orderByName.setOnAction(e -> {
            order = Order.ByName;
            update();
        });
        orderByAge.setOnAction(e -> {
            order = Order.ByAge;
            update();
        });
        HBox controlsBox = new HBox(10, filterAll, filterStudents, filterAthletes, orderByName, orderByAge);
        controlsBox.setAlignment(Pos.CENTER);
        Button exit = new Button("Exit");
        exit.setOnAction(e -> showConfirmation());
        VBox windowBox = new VBox(controlsBox, text, exit);
        windowBox.setSpacing(10);
        windowBox.setPadding(new Insets(10));
        Scene root = new Scene(windowBox, 600, 300);
        primaryStage.setTitle("Gym management");
        primaryStage.setScene(root);
        primaryStage.setOnCloseRequest(e -> {
            e.consume();
            showConfirmation();
        });
        primaryStage.show();
        update();
    }

    void update() {
        LinkedList<Subscriber> subscribersToShow = new LinkedList<>();
        filterAll.setDisable(false);
        filterStudents.setDisable(false);
        filterAthletes.setDisable(false);
        if (filter == Filter.All) {
            subscribersToShow = subscribers.getAll();
            filterAll.setDisable(true);
        }
        if (filter == Filter.Students) {
            subscribersToShow = subscribers.getStudents();
            filterStudents.setDisable(true);
        }
        if (filter == Filter.Athletes) {
            subscribersToShow = subscribers.getAthletes();
            filterAthletes.setDisable(true);
        }
        orderByAge.setDisable(false);
        orderByName.setDisable(false);
        if (order == Order.ByAge) {
            Collections.sort(subscribersToShow, Subscriber.ageComparator);
            orderByAge.setDisable(true);
        } else {
            Collections.sort(subscribersToShow, Subscriber.nameComparator);
            orderByName.setDisable(true);
        }
        text.setText(SubscriberInventory.listToString(subscribersToShow));
    }

    void showConfirmation() {
        ConfirmationBox c = new ConfirmationBox();
        c.show("Confirmation", "Do you want to quit?");
        if (c.yesClicked) stage.close();
    }

    public static void main(String[] args) {
        launch(args);
    }
}