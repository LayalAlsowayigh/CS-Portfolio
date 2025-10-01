package survey;

import java.io.*;
import java.util.*;

/**
 * Name: Layal Alsowayogh
 * Username: Alsoli01
 */

public class StateSurvey {

	// You cannot modify any of the provided code in any way!
	private static Scanner kb;

	public static void main(String[] args) {
		try {
			System.out.printf("Welcome to our survey. You may enter "
					+ "\"quit\" at any time to cancel the survey.\n\n");

			kb = new Scanner(System.in);
			
			int age = getAge();

			int ZIPCode = getZIPCode();

			String[][] states = readStateFile();

			String state = getState(states);

			System.out.printf("\nAge:\t\t%d\n", age);
			System.out.printf("Address:\t%s %s\n\n", ZIPCode, state.strip());

			System.out.printf("Your survey is complete!\n");
		}
		catch (CancelledSurveyException e) {
			System.out.printf("%s\n", e.getMessage());
		}
		finally {
			System.out.printf("Thank you for your time.");
			kb.close();
		}
	}

	//TODO Complete all the needed methods below this line
	//Reminder: Use only printf (with "\n" as needed) for all printing
	//a method to get the user age
	public static int getAge() throws CancelledSurveyException
	{
		while (true)
		{
			System.out.printf("Please enter your age: ");
			String response = kb.nextLine();
			if (response.equalsIgnoreCase("quit"))
			{
				throw new CancelledSurveyException();
			}

			try {
				int age = Integer.parseInt(response);
				if (age < 18)
				{
					//invild if the user is too young, under 18
					System.out.printf("You are too young to complete the survey.\n");
					throw new CancelledSurveyException();
				}
				return age;
			} catch (NumberFormatException e) {
				// invalid input if it not a num
				System.out.printf("You've entered an invalid age.\n");
			}
		}
	}
	//method to get the user zipcode
	public static int getZIPCode() throws CancelledSurveyException
	{
		while (true)
		{
			//user zip code
			System.out.printf("Please enter your ZIP code: ");
			String response = kb.nextLine();
			//input quit to exit
			if (response.equalsIgnoreCase("quit"))
			{
				throw new CancelledSurveyException();
			}

			try {
				// check if the zip code is valid and its 5 numbers
				int zip = Integer.parseInt(response);
				if (response.length() == 5 && zip > 0)
				{
					return zip;
				} else {
					//invalid zip code if the user enter zip code format
					System.out.printf("Invalid ZIP code.\n");
				}
			} catch (NumberFormatException e) {
				//invalid if its not a number
				System.out.printf("Invalid input.\n");
			}
		}
	}
	//method to read the state from the binary file
	public static String[][] readStateFile()
	{
		//read states abbreviation and the full name of the state the user enter
		String[][] states = new String[50][2];
		try (DataInputStream dis = new DataInputStream(new FileInputStream("states.bin")))
		{
			for (int i = 0; i < 50; i++)
			{
				//abbreviation
				states[i][0] = dis.readUTF();
				//full name
				states[i][1] = dis.readUTF();
			}
		} catch (IOException e)
		{
			// invild if theres and issue reading from the file
			System.out.printf("Error reading state file.\n");
		}
		return states;
	}
	//method to get the user state
	public static String getState(String[][] states) throws CancelledSurveyException
	{
		while (true)
		{
			System.out.printf("Please enter the 2-letter state abbreviation: ");
			String response = kb.nextLine();
			// if user wants to quit
			if (response.equalsIgnoreCase("quit"))
			{
				throw new CancelledSurveyException();
			}
			//check for the state in the array
			for (String[] state : states)
			{
				if (state[0].equalsIgnoreCase(response))
				{
					return state[1];
				}
			}
			//state is not found
			System.out.printf("The state abbreviation was not found.\n");
		}
	}
}
