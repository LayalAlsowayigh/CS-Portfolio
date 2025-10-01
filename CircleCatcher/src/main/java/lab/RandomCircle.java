package lab;

import javafx.animation.TranslateTransition;
import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;
import javafx.util.Duration;
import java.util.Random;

/**
 * Name: layal alsowayigh
 * Username: alsoli01
 */

public class RandomCircle extends Circle {

    private boolean captured;

    public RandomCircle(double paneWidth, double paneHeight) {
        Random random = new Random();

        // random radius and colors, random between 20 and 50
        this.setRadius(20 + random.nextInt(30));
        this.setFill(Color.color(random.nextDouble(), random.nextDouble(), random.nextDouble()));

        // Random position
        this.setCenterX(random.nextDouble() * (paneWidth - 2 * getRadius()) + getRadius());
        this.setCenterY(random.nextDouble() * (paneHeight - 2 * getRadius()) + getRadius());

        // random movement
        TranslateTransition transition = createBackAndForthAnimation(random, paneWidth, paneHeight);
        transition.play();

        // Move circle to the center on click
        this.setOnMousePressed(event -> {
            if (!captured) {
                // Stop
                transition.stop();
                //move to center
                moveToCenter(paneWidth / 2, paneHeight / 2);
                captured = true; // mark the circles
            }
        });
    }

    private TranslateTransition createBackAndForthAnimation(Random random, double paneWidth, double paneHeight) {
        TranslateTransition transition = new TranslateTransition(Duration.seconds(1.5), this);

        // Generate random points within bounds
        double targetX = random.nextDouble() * (paneWidth - 2 * getRadius()) + getRadius();
        double targetY = random.nextDouble() * (paneHeight - 2 * getRadius()) + getRadius();

        transition.setToX(targetX - getCenterX());
        transition.setToY(targetY - getCenterY());
        // goes Back and forth
        transition.setAutoReverse(true);
        transition.setCycleCount(TranslateTransition.INDEFINITE);

        return transition;
    }

    private void moveToCenter(double centerX, double centerY) {
        TranslateTransition transition = new TranslateTransition(Duration.seconds(0.5), this);
        // move to center X
        transition.setToX(centerX - this.getCenterX());
        // move to center Y
        transition.setToY(centerY - this.getCenterY());
        transition.play();
    }

    public boolean isCaptured() {
        return captured;
    }
}
