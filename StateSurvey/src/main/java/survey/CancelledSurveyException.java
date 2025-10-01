package survey;

/**
 * Name: Layal Alsowayigh
 * Username: Alsoli01
 */

public class CancelledSurveyException extends Exception
{
    public CancelledSurveyException()
    {
        //if the user answer is invalid
        super("Your survey was cancelled.");
    }
}

