package gui;

import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.scene.text.Text;
import javafx.stage.Modality;
import javafx.stage.Stage;

public class ConfirmationBox {
    public boolean yesClicked = false;
    public void show(String title, String text) {
        Stage stage = new Stage();
        stage.initModality(Modality.APPLICATION_MODAL);
        stage.setTitle(title);
        stage.setMinWidth(250);

        Text ta = new Text();
        ta.setText(text);

        Button btnYes = new Button();
        btnYes.setText("Yes");
        btnYes.setOnAction(e -> {
            yesClicked = true;
            stage.close();
        });
        Button btnNo = new Button();
        btnNo.setText("No");
        btnNo.setOnAction(e -> {
            yesClicked = false;
            stage.close();
        });

        HBox buttons = new HBox(5, btnYes, btnNo);
        VBox pane = new VBox(20, ta, buttons);
        buttons.setAlignment(Pos.CENTER);
        pane.setAlignment(Pos.CENTER);
        pane.setPadding(new Insets(10));

        Scene scene = new Scene(pane);
        stage.setScene(scene);
        stage.showAndWait();
    }
}
