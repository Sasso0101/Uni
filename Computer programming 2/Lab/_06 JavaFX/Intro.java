import javafx.application.*;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.scene.*;
import javafx.scene.layout.*;
import javafx.scene.control.*;
import javafx.stage.*;

public class Intro extends Application {
    @Override
    public void start(Stage primaryStage) {
        Button btn = new Button("Say Hello World!");
        btn.setOnAction(new EventHandler<ActionEvent>() {
            @Override
            public void handle(ActionEvent event) {
                if (btn.getText() == "Say Hello World!")
                    btn.setText("Hello World!");
            }
        });
        StackPane root = new StackPane();
        root.getChildren().add(btn);
        Scene scene = new Scene(root, 300, 250);
        primaryStage.setTitle("Hello world!");
        primaryStage.setScene(scene);
        primaryStage.show();
    }
    public static void main(String args[]) {
        launch(args);
    }
}
