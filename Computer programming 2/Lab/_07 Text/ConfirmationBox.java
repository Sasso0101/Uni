import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.stage.Modality;
import javafx.stage.Stage;

public class ConfirmationBox {
    static Stage stage;
    static boolean yesClicked = false;
    public static boolean show(String title, String message, String textYes, String textNo) {
        stage = new Stage();
        stage.initModality(Modality.APPLICATION_MODAL);
        stage.setTitle(title);
        stage.setMinHeight(100);
        stage.setMinWidth(100);
        Label label = new Label(message);
        Button btnYes = new Button(textYes);
        btnYes.setOnAction(e -> yesClicked());
        Button btnNo = new Button(textNo);
        btnNo.setOnAction(e -> noClicked());
        HBox btnPane = new HBox(btnYes, btnNo);
        btnPane.setAlignment(Pos.CENTER);
        VBox mainPane = new VBox(label, btnPane);
        mainPane.setAlignment(Pos.CENTER);
        Scene scene = new Scene(mainPane);
        stage.setScene(scene);
        stage.showAndWait();
        return yesClicked;
    }

    private static void yesClicked() {
        stage.close();
        yesClicked = true;
    }
    private static void noClicked() {
        stage.close();
        yesClicked = false;
    }
}
