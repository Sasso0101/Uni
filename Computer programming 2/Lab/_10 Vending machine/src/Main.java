import data.Product;
import data.VendingMachine;
import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TextArea;
import javafx.scene.control.TextField;
import javafx.scene.layout.FlowPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.stage.Stage;

import java.util.ArrayList;
import java.util.Collections;

public class Main extends Application {

    public static void main(String[] args) {
        launch(args);
    }

    private TextField buyField = new TextField();
    private TextField buyQuantityField = new TextField();
    private TextArea receipt = new TextArea();
    private VendingMachine machine = new VendingMachine("products.txt");
    private TextField temperatureField = new TextField(String.valueOf(machine.getTemperature()));
    private TextArea management = new TextArea();
    @Override
    public void start(Stage primaryStage) {
        receipt.setEditable(false);
        receipt.setStyle("-fx-font-family: monospace");
        Button buyButton = new Button("Buy");
        buyButton.setOnAction(e -> buy());
        HBox buyControls = new HBox(new Label("Enter ID: "), buyField, new Label("Quantity: "), buyQuantityField, buyButton);

        Button setTemperature = new Button("Set temperature");
        setTemperature.setOnAction(e -> setTemperature());
        HBox temperatureControls = new HBox(new Label("New temperature"), temperatureField, setTemperature);

        Button priceListDesc = new Button("Price list (ordered alphabetically)");
        priceListDesc.setOnAction(e -> showPriceList("description"));
        Button priceListCost = new Button("Price list (ordered by price)");
        priceListCost.setOnAction(e -> showPriceList("price"));
        Button deliveredDate = new Button("Delivered products (ordered by date)");
        deliveredDate.setOnAction(e -> showDeliveredProducts("date"));
        Button deliveredCost = new Button("Delivered products (ordered by total price)");
        deliveredCost.setOnAction(e -> showDeliveredProducts("price"));
        Button presentExp = new Button("Products present (ordered by exp. date)");
        presentExp.setOnAction(e -> showProductsPresent("date"));
        Button presentCode = new Button("Products present (ordered by code)");
        presentCode.setOnAction(e -> showProductsPresent("code"));
        FlowPane managementControls = new FlowPane(priceListDesc, priceListCost, deliveredDate, deliveredCost, presentExp, presentCode);
        VBox managementPanel = new VBox(management, managementControls);
        management.setEditable(false);
        management.setStyle("-fx-font-family: monospace");

        VBox buyPanel = new VBox(buyControls, receipt, temperatureControls, managementPanel);
        buyPanel.setSpacing(10);
        Scene root = new Scene(buyPanel);
        primaryStage.setScene(root);
        primaryStage.show();
    }

    private void buy() {
        receipt.setText(machine.buyAndGetReceipt(buyField.getText(), buyQuantityField.getText()));
    }
    private void setTemperature() {
        machine.setTemperature(Integer.parseInt(temperatureField.getText()));
    }

    private void showPriceList(String order) {
        if (order.equals("description")) Collections.sort(machine.getProducts(), VendingMachine.priceListDescriptionComparator);
        else if (order.equals("price")) Collections.sort(machine.getProducts(), VendingMachine.priceListPriceComparator);
        management.setText(machine.productsToString());
    }

    private void showProductsPresent(String order) {
        ArrayList<Product> productsPresent = machine.getProductsPresent();
        if (order.equals("date")) Collections.sort(productsPresent, VendingMachine.priceListExpDateComparator);
        else if (order.equals("code")) Collections.sort(productsPresent, VendingMachine.priceListIdComparator);
        management.setText(VendingMachine.productsToString(productsPresent));
    }

    private void showDeliveredProducts(String order) {
        if (order.equals("date")) Collections.sort(machine.getTransactions(), VendingMachine.transactionsDateComparator);
        else if (order.equals("price")) Collections.sort(machine.getTransactions(), VendingMachine.transactionsPriceComparator);
        management.setText(machine.transactionsToString());
    }
}
