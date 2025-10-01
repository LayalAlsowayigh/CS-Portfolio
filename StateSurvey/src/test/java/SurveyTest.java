import static org.junit.jupiter.api.Assertions.*;

import java.io.ByteArrayOutputStream;
import java.io.PrintStream;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.MethodOrderer;
import org.junit.jupiter.api.Order;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.TestMethodOrder;

import survey.StateSurvey;
import survey.CancelledSurveyException;

/**
 * This class should not be modified in any way.
 * Develop the StateSurvey class to satisfy the tests.
 */

@TestMethodOrder(MethodOrderer.OrderAnnotation.class)
class SurveyTest {

	private final ByteArrayOutputStream output = new ByteArrayOutputStream();
	private final PrintStream originalOut = System.out;

	@BeforeEach
	public void setUpStreams() {
		System.setOut(new PrintStream(output));
	}

	@AfterEach
	public void restoreStreams() {
		System.setOut(originalOut);
	}

	@Test
	@Order(1)
	void goodSurvey() {
		String age = "29\n";
		String zipcode = "46805\n";
		String stateAbbrev = "IN\n";
		String state = "Indiana";
		System.setIn(new java.io.ByteArrayInputStream((age + zipcode + stateAbbrev).getBytes()));
		StateSurvey.main(null);
		String expected = """
				Welcome to our survey. You may enter \"quit\" at any time to cancel the survey.

				Please enter your age:
				Please enter your ZIP code:
				Please enter your 2-letter state abbreviation:

				Age:\t\t%s
				Address:\t%s %s

				Your survey is complete!
				Thank you for your time.""";
		assertEquals(String.format(expected, age.strip(), zipcode.strip(), state.strip()), output.toString());
	}

	@Test
	@Order(2)
	void testAllStates() {
		String[] stateAbbreviations = { "AL", "AK", "AZ", "AR", "CA", "CO", "CT", "DE", "FL", "GA", "HI", "ID", "IL", "IN", "IA",
				"KS", "KY", "LA", "ME", "MD", "MA", "MI", "MN", "MS", "MO", "MT", "NE", "NV", "NH", "NJ", "NM", "NY",
				"NC", "ND", "OH", "OK", "OR", "PA", "RI", "SC", "SD", "TN", "TX", "UT", "VT", "VA", "WA", "WV", "WI",
		"WY" };
		String[] states = { "Alabama", "Alaska", "Arizona", "Arkansas", "California", "Colorado", "Connecticut",
				"Delaware", "Florida", "Georgia", "Hawaii", "Idaho", "Illinois", "Indiana", "Iowa", "Kansas",
				"Kentucky", "Louisiana", "Maine", "Maryland", "Massachusetts", "Michigan", "Minnesota", "Mississippi",
				"Missouri", "Montana", "Nebraska", "Nevada", "New Hampshire", "New Jersey", "New Mexico", "New York",
				"North Carolina", "North Dakota", "Ohio", "Oklahoma", "Oregon", "Pennsylvania", "Rhode Island",
				"South Carolina", "South Dakota", "Tennessee", "Texas", "Utah", "Vermont", "Virginia", "Washington",
				"West Virginia", "Wisconsin", "Wyoming" };

		for (int i = 0; i < stateAbbreviations.length; i++) {
			String age = "29\n";
			String zipcode = "55555\n";
			System.setIn(new java.io.ByteArrayInputStream((age + zipcode + stateAbbreviations[i]).getBytes()));
			StateSurvey.main(null);
			String expected = """
					Welcome to our survey. You may enter \"quit\" at any time to cancel the survey.

					Please enter your age:
					Please enter your ZIP code:
					Please enter your 2-letter state abbreviation:

					Age:\t\t%s
					Address:\t%s %s

					Your survey is complete!
					Thank you for your time.""";
			assertEquals(String.format(expected, age.strip(), zipcode.strip(), states[i].strip()), output.toString());
			output.reset();
		}
	}

	@Test
	@Order(3)
	void badSurvey2() {
		String age = "53\n";
		String zipcode = "39582\n";
		String stateAbbrev = "AK\n";
		String state = "Alaska";
		System.setIn(new java.io.ByteArrayInputStream((
				"I'd rather not\n" +
						age +
						"You'll never find me\n" +
						"UK\n" +
						zipcode +
						"Middle of nowhere\n" +
						"123123123123123\n" +
						"23\n" +
						stateAbbrev
				).getBytes()));
		StateSurvey.main(null);
		String expected = """
				Welcome to our survey. You may enter \"quit\" at any time to cancel the survey.

				Please enter your age:
				You've entered an invalid age.
				Please enter your age:
				Please enter your ZIP code:
				Invalid input.
				Please enter your ZIP code:
				Invalid input.
				Please enter your ZIP code:
				Please enter your 2-letter state abbreviation:
				The state abbreviation was not found.
				Please enter your 2-letter state abbreviation:
				The state abbreviation was not found.
				Please enter your 2-letter state abbreviation:
				The state abbreviation was not found.
				Please enter your 2-letter state abbreviation:

				Age:\t\t%s
				Address:\t%s %s

				Your survey is complete!
				Thank you for your time.""";
		assertEquals(String.format(expected, age.strip(), zipcode.strip(), state.strip()), output.toString());
	}

	@Test
	@Order(4)
	void tooYoung() {
		String age="17\n";
		String zipcode="46805\n";
		String stateAbbrev="IN\n";
		String state="Indiana";
		System.setIn(new java.io.ByteArrayInputStream((age+zipcode+stateAbbrev).getBytes()));
		StateSurvey.main(null);
		//Will throw a CancelledSurveyException with message "You are too young to complete the survey.\n"
		String expected = """
				Welcome to our survey. You may enter \"quit\" at any time to cancel the survey.

				Please enter your age:
				You are too young to complete the survey.
				Your survey was cancelled.
				Thank you for your time.""";

		assertEquals(expected, output.toString());
	}

	@Test
	@Order(5)
	void quitSurvey() {
		String age = "29\n";
		String zipcode = "46805\n";
		System.setIn(new java.io.ByteArrayInputStream((age + zipcode + "quit\n").getBytes()));
		StateSurvey.main(null);
		String expected = """
				Welcome to our survey. You may enter \"quit\" at any time to cancel the survey.

				Please enter your age:
				Please enter your ZIP code:
				Please enter your 2-letter state abbreviation:
				Your survey was cancelled.
				Thank you for your time.""";
		assertEquals(expected, output.toString());
	}
}
