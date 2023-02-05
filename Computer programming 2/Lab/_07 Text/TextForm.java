import javafx.application.*;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.TextArea;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.HBox;
import javafx.stage.Stage;

public class TextForm extends Application {
    private Stage stage;
    public void start(Stage stage) {
        this.stage = stage;
        TextArea text = new TextArea("This is init text");
        Button button = new Button("Click me");
        button.setOnAction(new EventHandler<ActionEvent>() {
            int line = 0;
            @Override
            public void handle(ActionEvent e) {
                text.appendText("\nLine " + ++line);
            }
        });
        Button exit = new Button("Exit");
        exit.setOnAction(e -> closeWindow());
        HBox buttonBox = new HBox();
        buttonBox.setAlignment(Pos.CENTER);
        buttonBox.getChildren().addAll(button, exit);

        BorderPane root = new BorderPane();
        root.setCenter(text);
        root.setBottom(buttonBox);
        Scene scene = new Scene(root, 300, 250);
        stage.setTitle("TextArea demo");
        stage.setScene(scene);
        stage.setOnCloseRequest(e -> {
            e.consume();
            closeWindow();
        });
        stage.show();
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
