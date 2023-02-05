package gui;

import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.TextArea;
import javafx.scene.layout.VBox;
import javafx.stage.Modality;
import javafx.stage.Stage;

public class MessageBox {
    public static void show(String text, String title)
    {
        Stage stage = new Stage();
        stage.initModality(Modality.APPLICATION_MODAL);
        stage.setTitle(title);
        stage.setMinWidth(250);

        TextArea ta = new TextArea();
        ta.setEditable(false);
        ta.setText(text);

        Button btnOK = new Button();
        btnOK.setText("OK");
        btnOK.setOnAction(e -> stage.close());

        VBox pane = new VBox(20);
        pane.getChildren().addAll(ta, btnOK);
        pane.setAlignment(Pos.CENTER);

        Scene scene = new Scene(pane);
        stage.setScene(scene);
        stage.showAndWait();
    }
}
