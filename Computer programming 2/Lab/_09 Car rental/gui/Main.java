package gui;

import data.car.CarInventory;
import data.rent.Rent;
import data.rent.RentInventory;
import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.ChoiceBox;
import javafx.scene.control.Label;
import javafx.scene.control.TextArea;
import javafx.scene.control.TextField;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.stage.Stage;

public class Main extends Application {
    private CarInventory carInventory = new CarInventory("pricelist.txt");
    private ChoiceBox<String> carFilter = new ChoiceBox<String>();
    private ChoiceBox<String> carSort = new ChoiceBox<String>();
    private TextArea carList = new TextArea();

    private RentInventory rentInventory = new RentInventory("persone.txt");
    private ChoiceBox<String> rentSort = new ChoiceBox<String>();
    private TextArea rentList = new TextArea();

    @Override
    public void start(Stage stage) {
        carList.setMinWidth(800);
        carList.setMinHeight(200);
        carList.setStyle("-fx-font-family: monospace");
        carList.setEditable(false);
        carList.setText(carInventory.toString());
        HBox carControls = new HBox(5);
        carFilter.getItems().addAll("A", "B", "C", "D", "E", "F", "All");
        carFilter.setOnAction(e -> updateCarList());
        carSort.getItems().addAll("By price", "By manufacturer", "By gearbox+price");
        carSort.setOnAction(e -> updateCarList());
        carControls.getChildren().addAll(new Label("Filter"), carFilter, new Label("Sort"), carSort);
        BorderPane carsPane = new BorderPane();
        carsPane.setTop(new Label("Car inventory"));
        carsPane.setCenter(carList);
        carsPane.setBottom(carControls);

        rentList.setMinWidth(800);
        rentList.setMinHeight(200);
        rentList.setStyle("-fx-font-family: monospace");
        rentList.setEditable(false);
        rentList.setText(rentInventory.toString());
        HBox rentControls = new HBox(5);
        rentSort.getItems().addAll("By surname+name", "By renting date ascending", "By renting date descending");
        rentSort.setOnAction(e -> updateRentList());
        rentControls.getChildren().addAll(new Label("Sort"), rentSort);
        BorderPane rentsPane = new BorderPane();
        rentsPane.setTop(new Label("Current rents"));
        rentsPane.setCenter(rentList);
        rentsPane.setBottom(rentControls);

        TextField returnField = new TextField();
        Button returnButton = new Button("Return car");
        returnButton.setOnAction(e -> returnCar(returnField.getText()));
        HBox returnBox = new HBox(new Label("Enter ID: "), returnField, returnButton);

        VBox root = new VBox(carsPane, rentsPane, returnBox);
        root.setSpacing(10);
        Scene scene = new Scene(root);
        stage.setScene(scene);
        stage.show();
    }

    private void updateCarList() {
        String classCode = carFilter.getValue();
        String sort = carSort.getValue();
        CarInventory filteredCars;
        if (classCode == null || classCode.equals("All")) filteredCars = carInventory;
        else filteredCars = carInventory.filter(classCode.charAt(0));
        
        if (sort != null) {
            if (sort.equals("By price")) filteredCars.sort(CarInventory.priceComparator);
            if (sort.equals("By manufacturer")) filteredCars.sort(CarInventory.manufacturerComparator);
            if (sort.equals("By gearbox+price")) filteredCars.sort(CarInventory.gearboxPriceComparator);
        }
        carList.setText(filteredCars.toString());
    }

    private void updateRentList() {
        String sort = rentSort.getValue();
        if (sort != null) {
            if (sort.equals("By surname+name")) rentInventory.sort(RentInventory.nameComparator);
            if (sort.equals("By renting date ascending")) rentInventory.sort(RentInventory.dateComparatorAsc);
            if (sort.equals("By renting date descending")) rentInventory.sort(RentInventory.dateComparatorDesc);
        }
        rentList.setText(rentInventory.toString());
    }

    private void returnCar(String id) {
        Rent r = rentInventory.getRent(Integer.parseInt(id));
        String receipt = rentInventory.getReceipt(carInventory, r);
        MessageBox.show(receipt, "Receipt");
        rentInventory.removeRent(r);
        updateRentList();
    }

    public static void main(String args[]) {
        launch(args);
    }
}
