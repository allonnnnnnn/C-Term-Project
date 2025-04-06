#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hospital.h"

#define CURRENT_YEAR 2025

/**
 * Converts a Date struct into a string formatted as day/month/year.
 *
 * @param date Pointer to a Date struct. If NULL, the function does nothing.
 * @param string Buffer to hold the resulting date string (e.g., "1/1/2025").
 */
void dateToString(const struct Date* date, char* string)
{
    if (date == NULL)
    {
        return;
    }

    char dayString[10];
    char monthString[10];
    char yearString[10];

    sprintf(dayString, "%d", date->day);
    sprintf(monthString, "%d", date->month);
    sprintf(yearString, "%d", date->year);

    strcpy(string, "");
    strcat(string, dayString);
    strcat(string, "/");
    strcat(string, monthString);
    strcat(string, "/");
    strcat(string, yearString);
}

/**
 * Continuously prompts the user for day, month, and year until a valid date is entered.
 * The valid date is then assigned to the passed-in Date struct.
 *
 * @param date Pointer to the Date struct to initialize.
 */
void promptDateInitialization(struct Date* date)
{
    int dateInitializationStatus = 0;
    int year = 0;
    int month = 0;
    int day = 0;

    while (dateInitializationStatus == 0)
    {
        printf("\nPlease input the day:\n");
        scanf("%d", &day);

        printf("\nPlease input the month:\n");
        scanf("%d", &month);

        printf("\nPlease input the year:\n");
        scanf("%d", &year);

        dateInitializationStatus = initializeDate(date, day, month, year);

        if (dateInitializationStatus == 0)
        {
            printf("Please try again\n");
        }
    }
}

/**
 * Initializes a Date struct with the given day, month, and year, provided it's a valid date.
 *
 * @param date Pointer to the Date struct to be initialized.
 * @param day The day of the month.
 * @param month The month (1-12).
 * @param year The year (must pass validateDate check).
 * @return 1 if successful, 0 if validation fails.
 */
int initializeDate(struct Date* date, const int day, const int month, const int year)
{
    if (!validateDate(day, month, year))
    {
        return 0;
    }

    date->day = day;
    date->month = month;
    date->year = year;

    return 1;
}

/**
 * Parses a date string of the form "day/month/year" into a newly allocated Date struct.
 *
 * @param date Pointer to a Date* that will be allocated and populated.
 * @param dateString The input string containing the date in the format "d/m/yyyy".
 */
void parseDate(struct Date** date, char* dateString)
{
    int day = 0;
    int month = 0;
    int year = 0;
    int dateSection = 0;
    char* endptr;

    // Tokenize by '/'
    char* token = strtok(dateString, "/");

    while (token != NULL && dateSection < 3)
    {
        switch (dateSection)
        {
        case 0:
            day = (int)strtol(token, &endptr, 10);
            break;
        case 1:
            month = (int)strtol(token, &endptr, 10);
            break;
        case 2:
            year = (int)strtol(token, &endptr, 10);
            break;
        default: ; // Should never get here, but included for clarity
        }

        // If there was any non-numeric data in the token
        if (*endptr != '\0')
        {
            printf("%s", token);
            printf("Couldn't convert data properly. Please check the dateString\n");
            return;
        }

        token = strtok(NULL, "/");
        dateSection++;
    }

    *date = malloc(sizeof(struct Date));

    if (*date == NULL)
    {
        printf("Could not allocate memory for date\n");
        return;
    }

    (*date)->day = day;
    (*date)->month = month;
    (*date)->year = year;
}

/**
 * Checks if the provided day, month, and year form a valid date.
 * Includes a leap-year check to allow February 29 on leap years.
 *
 * @param day The day of the month.
 * @param month The month (1-12).
 * @param year The year (between 1900 and CURRENT_YEAR, inclusive).
 * @return 1 if valid, 0 otherwise.
 */
int validateDate(const int day,
                 const int month,
                 const int year)
{
    // Basic range checks for year
    if (year < 1900 || year > CURRENT_YEAR)
    {
        printf("Invalid Year\n");
        return 0;
    }

    // Basic range check for month
    if (month < 1 || month > 12)
    {
        printf("Invalid Month\n");
        return 0;
    }

    // Days in each month (non-leap year) by default
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Adjust for leap year in February
    if ((year % 400 == 0) || (year % 4 == 0 && year % 100 != 0))
    {
        daysInMonth[1] = 29;
    }

    //Will return 0 if the date is lower than 1 and higher than whatever the upper bound is according to the month
    if (day < 1 || day > daysInMonth[month - 1])
    {
        printf("Invalid Day\n");
        return 0;
    }

    return 1;
}
