#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hospital.h"

#define CURRENT_YEAR 2025

void dateToString(const struct Date* date,
                  char* string)
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

void promptDateInitialization(struct Date* date)
{
    int dateInitializationStatus;
    int year;
    int month;
    int day;

    dateInitializationStatus = 0;
    year = 0;
    month = 0;
    day = 0;

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

int initializeDate(struct Date* date,
                   const int day,
                   const int month,
                   const int year)
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

void parseDate(struct Date** date,
               char* dateString)
{
    int day = 0;
    int month = 0;
    int year = 0;
    int dateSection = 0;
    char* endptr;

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
        default: ;
        }

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
        printf("Could not allocate memory to date\n");
        return;
    }

    (*date)->day = day;
    (*date)->month = month;
    (*date)->year = year;
}

int validateDate(const int day,
                 const int month,
                 const int year)
{
    if (year < 1900 || year > CURRENT_YEAR)
    {
        printf("Invalid Year\n");
        return 0;
    }

    if (month < 1 || month > 12)
    {
        printf("Invalid Month\n");
        return 0;
    }

    // Days in each month
    const int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (day < 1 || day > daysInMonth[month - 1])
    {
        printf("Invalid Day\n");
        return 0;
    }

    return 1;
}


