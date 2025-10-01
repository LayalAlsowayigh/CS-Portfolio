package lab;

import javafx.animation.KeyFrame;
import javafx.animation.Timeline;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.CheckMenuItem;
import javafx.scene.control.Label;
import javafx.scene.layout.Pane;
import javafx.util.Duration;
import java.util.ArrayList;
import java.util.List;

/**
 * Name: layal alsowayigh
 * Username: alsoli01
 */

public class Controller {

    @FXML
    private Pane gamePane;

    @FXML
    private Label titleLabel;

    @FXML
    private Button playButton;

    @FXML
    private CheckMenuItem normalMode; // extra points

    @FXML
    private CheckMenuItem extraMode;// extra points

    @FXML
    private Label instructionsLabel1;

    @FXML
    private Label instructionsLabel2;

    private List<RandomCircle> circles = new ArrayList<>();
    private Timeline extraModeTimeline;//extra points
    private Timeline checkGameTimeline;

    public void initialize() {
        // modes
        normalMode.setSelected(true);//extra points
        extraMode.setSelected(false);// extra points

        checkGameTimeline = new Timeline(new KeyFrame(Duration.seconds(0.5), event -> {
            // Check if all circles are captured
            if (circles.stream().allMatch(RandomCircle::isCaptured) && !circles.isEmpty()) {
                if (extraModeTimeline != null) {
                    extraModeTimeline.stop(); // etxra points
                }
                titleLabel.setVisible(true); // You Win label
                playButton.setText("Play Again"); //Play Again button
                playButton.setVisible(true); // Play Again button
                instructionsLabel1.setVisible(true); //  instruction label
                instructionsLabel2.setVisible(true); //instruction label
            }
        }));
        checkGameTimeline.setCycleCount(Timeline.INDEFINITE);
        checkGameTimeline.play();
    }

    public void startGame() {
        // Reset game state
        gamePane.getChildren().clear();
        circles.clear();
        titleLabel.setVisible(false);
        playButton.setVisible(false);
        instructionsLabel1.setVisible(false);
        instructionsLabel2.setVisible(false);
        // Start game in mode the player choose
        if (extraMode.isSelected()) {
            startExtraMode(1);
        } else {
            // normal mode
            generateCircles(10);
        }
    }

    private void generateCircles(int count) {
        for (int i = 0; i < count; i++) {
            RandomCircle circle = new RandomCircle(gamePane.getWidth(), gamePane.getHeight());
            circles.add(circle);
            gamePane.getChildren().add(circle);
        }
    }

    private void startExtraMode(double intervalInSeconds) {
        if (extraModeTimeline != null) {
            extraModeTimeline.stop();
        }

        // Extra mode
        //extra points
        extraModeTimeline = new Timeline(new KeyFrame(Duration.seconds(intervalInSeconds), event -> {
                generateCircles(1);
        }));
        extraModeTimeline.setCycleCount(Timeline.INDEFINITE);
        extraModeTimeline.play();
    }


    private void stopExtraMode() {
        if (extraModeTimeline != null) {
            extraModeTimeline.stop();
        }
    }
    //extra points
    public void selectNormalMode() {
        if (normalMode.isSelected()) {
            extraMode.setSelected(false);
        }
    }
    //extra points
    public void selectExtraMode() {
        if (extraMode.isSelected()) {
            normalMode.setSelected(false);
        }
    }
}
