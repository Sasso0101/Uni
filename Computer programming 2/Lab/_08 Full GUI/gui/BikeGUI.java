package gui;

import data.BikeCatalog;
import javafx.application.*;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.TextArea;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.HBox;
import javafx.stage.Stage;

public class BikeGUI extends Application {
    private BikeCatalog catalog = new BikeCatalog();
    private Stage stage;

    private Button clear = new Button("Clear");
    private Button importCatalog = new Button("Import catalog");
    private Button displayCatalog = new Button("Display catalog");
    private Button orderCatalog = new Button("Sort by wheel size");
    private Button exit = new Button("Exit");
    private TextArea text;

    @Override
    public void start(Stage stage) {
        stage.setTitle("Bike catalog");
        this.stage = stage;

        clear.setOnAction(e -> clear());
        importCatalog.setOnAction(e -> importCatalog());
        displayCatalog.setOnAction(e -> displayCatalog());
        orderCatalog.setOnAction(e -> orderCatalog());
        exit.setOnAction(e -> closeWindow());
        HBox buttonBox = new HBox(5);
        buttonBox.setAlignment(Pos.CENTER);
        buttonBox.getChildren().addAll(clear, importCatalog, displayCatalog, orderCatalog, exit);
        text = new TextArea();
        text.setEditable(false);
        BorderPane root = new BorderPane();
        root.setCenter(text);
        root.setBottom(buttonBox);
        BorderPane.setMargin(buttonBox, new Insets(10));
        Scene scene = new Scene(root, 500, 250);
        stage.setOnCloseRequest(e -> {
            e.consume();
            closeWindow();
        });
        stage.setScene(scene);
        stage.show();
    }

    private void clear() {
        text.setText("");
    }
    private void importCatalog() {
        catalog.importFromCSV("../bikes.txt");
    }
    private void displayCatalog() {
        text.setText(catalog.toString());
    }
    private void orderCatalog() {
        catalog.sortByWheelSize();
        clear();
        displayCatalog();
    }
    private void closeWindow() {
        boolean reallyQuit = false;
        reallyQuit = ConfirmationBox.show("Confirmation", "Do you really want to quit?", "Yes", "No");
        if (reallyQuit) {
            stage.close();
        }
    }

    public static void main(String args[]) {
        launch(args);
    }
}
