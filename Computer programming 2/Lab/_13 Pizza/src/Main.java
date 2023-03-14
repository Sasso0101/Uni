import data.AddOnIngredient;
import data.Order;
import data.Pizza;
import gui.MessageBox;
import javafx.application.Application;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.layout.FlowPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.stage.Stage;
import model.Menu;
import model.OrderByName;
import model.OrderByPrice;
import model.Orders;

import java.util.Collections;
import java.util.LinkedList;

public class Main extends Application {
    Menu menu = new Menu();
    Orders orders = new Orders();
    TextArea text = new TextArea();
    TextArea ordersText = new TextArea();
    TextField pizzaName = new TextField();
    ChoiceBox<AddOnIngredient> addon = new ChoiceBox<>();

    int filter = 0;
    int order = 0;

    @Override
    public void start(Stage primaryStage) {
        text.setEditable(false);
        text.setWrapText(true);
        Button showAll = new Button("Show all pizzas");
        showAll.setOnAction(e -> {
            filter = 0;
            updateMenu();
        });
        Button showVegan = new Button("Show vegan pizzas");
        showVegan.setOnAction(e -> {
            filter = 1;
            updateMenu();
        });
        Button showNonVegan = new Button("Show non vegan pizzas");
        showNonVegan.setOnAction(e -> {
            filter = 2;
            updateMenu();
        });
        Button orderByPrice = new Button("Order by price");
        orderByPrice.setOnAction(e -> {
            order = 1;
            updateMenu();
        });
        Button orderByName = new Button("Order by name");
        orderByName.setOnAction(e -> {
            order = 0;
            updateMenu();
        });
        FlowPane controls = new FlowPane(showAll, showVegan, showNonVegan, orderByName, orderByPrice);
        controls.setHgap(10);
        controls.setVgap(10);
        controls.setAlignment(Pos.CENTER);
        Button orderButton = new Button("Order");
        orderButton.setOnAction(e -> addOrder());
        HBox order = new HBox(new Label("Enter pizza name: "), pizzaName, addon, orderButton);
        order.setSpacing(10);
        VBox root = new VBox(controls, text, order, ordersText);
        root.setSpacing(10);
        root.setPadding(new Insets(10));
        Scene scene = new Scene(root);
        primaryStage.setTitle("Pizzeria Mamma mia");
        primaryStage.setScene(scene);
        initializeData();
        primaryStage.show();
    }

    private void initializeData() {
        menu.importMenu("menu.txt");
        menu.importAddOns("addons.txt");
        addon.getItems().addAll(menu.getAddOns());
        addon.getItems().add(null);
        updateMenu();
    }

    private void updateMenu() {
        LinkedList<Pizza> filteredMenu = switch (filter) {
            case 0 -> menu.getMenu();
            case 1 -> menu.getVegan();
            case 2 -> menu.getNonVegan();
            default -> new LinkedList<>();
        };
        switch (order) {
            case 0 -> Collections.sort(filteredMenu, new OrderByName());
            case 1 -> Collections.sort(filteredMenu, new OrderByPrice());
        }
        text.setText(menu.toString(filteredMenu) + addons());
    }

    private String addons() {
        return "\nADDONS:\n" + menu.addOnsToString();
    }

    private void addOrder() {
        String pizzaName = this.pizzaName.getText();
        Pizza p = menu.getPizza(pizzaName);
        if (p != null) {
            Order o = orders.createAndAddOrder(p, addon.getValue());
            updateOrders();
            if (o.getAddon() != null) showMessageBox("Created order", String.format("Pizza: %1$s\nAddon: %2$s\nPrice: %3$d DKK\nDatetime: %4$td/%4$tm/%4$ty %4$tR", o.getPizza().toString(), o.getAddon(), o.getPrice(), o.getDateTime()));
            else showMessageBox("Created order", String.format("Pizza: %1$s\nPrice: %2$d DKK\nDatetime: %3$td/%3$tm/%3$ty %3$tR", o.getPizza().toString(), o.getPrice(), o.getDateTime()));
        }
    }

    private void showMessageBox(String title, String message) {
        MessageBox.show(title, message);
    }

    private void updateOrders() {
        ordersText.setText(orders.toString());
    }

    public static void main(String[] args) {
        launch(args);
    }
}