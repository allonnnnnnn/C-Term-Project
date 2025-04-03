//
// Created by allan on 2025-03-28.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hospital.h"

#define CURRENT_YEAR 2025

int main()
{
    struct LinkedList patientList = {NULL, 0};
    int input = 1;

    //Loading previous saved patients
    loadSaveFile(&patientList);

    while (input != 6)
    {
        printf("\nMAIN MENU\n"
            "1. Add Patient Record\n"
            "2. Reporting and Analytics Menu\n"
            "3. Search Patient Menu\n"
            "4. Discharge Patient\n"
            "5. Manage Doctor Schedule\n"
            "6. Exit\n"
            "Please input your option:\n");

        scanf("%d", &input);

        if (input < 0 || input > 6)
        {
            printf("Please input a valid option\n");
            continue;
        }

        switch (input)
        {
        case 1:
            addPatient(&patientList);
            break;
        case 2:
            ReportsAndAnalyticsMenu(&patientList);
            break;
        case 3:
            searchPatientMenu(&patientList);
            break;
        case 4:
            dischargePatient(&patientList);
            break;
        case 5:
            break;
        case 6:
            writeAllPatientsToSaveFile(&patientList);
            break;
        }
    }

    //TODO: free up all the nodes' memory within the linkedlist, for you lil arshy
}

void replaceSpacesWithUnderscores(char* string)
{
    for (int i = 0; i < strlen(string); i++)
    {
        if (string[i] == 0)
        {
            break;
        }

        if (string[i] == ' ')
        {
            string[i] = '_';
        }
    }
}

void replaceUnderscoresWithSpaces(char* string)
{
    for (int i = 0; i < strlen(string); i++)
    {
        if (string[i] == 0)
        {
            break;
        }

        if (string[i] == '_')
        {
            string[i] = ' ';
        }
    }
}

void loadSaveFile(struct LinkedList* patientList)
{
    FILE* fptr;

    fptr = fopen("saveFile.txt", "r");

    if (fptr == NULL)
    {
        fopen("saveFile.txt", "w");
    }
    else
    {
        int id;
        char name[50];
        int age;
        char diagnosis[50];
        int roomNumber;
        char dateString[10];

        fscanf(fptr, "%d %s %d %s %d %s",
               &id, name, &age, diagnosis, &roomNumber, dateString);

        while (!feof(fptr))
        {
            struct Date* dischargedDate = NULL;

            if (strcmp(dateString, "null") != 0)
            {
                parseDate(&dischargedDate, dateString);
            }

            createPatientToList(patientList, id, name, age, diagnosis, roomNumber, dischargedDate);

            fscanf(fptr, "%d %s %d %s %d %s",
                   &id, name, &age, diagnosis, &roomNumber, dateString);
        }

        printf("Successfully loaded data\n");
        fclose(fptr);
    }
}

void writePatientToSaveFile(int id,
                            char name[50],
                            int age,
                            char diagnosis[50],
                            int roomNumber)
{
    FILE* fptr = fopen("saveFile.txt", "a");

    replaceSpacesWithUnderscores(name);
    replaceSpacesWithUnderscores(diagnosis);

    fprintf(fptr, "%d %s %d %s %d %s\n", id, name, age, diagnosis, roomNumber, "null");

    fclose(fptr);
}

void writeAllPatientsToSaveFile(const struct LinkedList* patientList)
{
    FILE* fptr = fopen("saveFile.txt", "w");
    struct listNode* currentNode = patientList->head;

    while (currentNode != NULL)
    {
        char dateString[10] = "null";
        char name[50];
        char diagnosis[50];

        dateToString(currentNode->patient->dischargeDate, dateString);

        strcpy(name, currentNode->patient->name);
        strcpy(diagnosis, currentNode->patient->diagnosis);
        replaceSpacesWithUnderscores(name);
        replaceSpacesWithUnderscores(diagnosis);

        fprintf(fptr, "%d %s %d %s %d %s\n",
                currentNode->patient->id, name,
                currentNode->patient->age, diagnosis,
                currentNode->patient->roomNumbers, dateString);

        currentNode = currentNode->next;
    }

    fclose(fptr);
}

int validateId(const struct LinkedList* patientList,
               int id)
{
    struct listNode* current = patientList->head;
    while (current != NULL)
    {
        if (current->patient->id == id)
        {
            return 1;
        }

        current = current->next;
    }

    return 0;
}

void addPatient(struct LinkedList* patientList)
{
    int id;
    char name[50];
    int age;
    char diagnosis[50];
    int roomNumber;

    int idValid = 0;
    int nameValid = 0;
    int ageValid = 0;
    int diagnosisValid = 0;
    int roomNumberValid = 0;

    while (idValid == 0)
    {
        printf("\nEnter in the patient Id:\n");
        scanf("%d", &id);
        getchar();

        if (id < 0 || validateId(patientList, id))
        {
            printf("\nInvalid input or Id is taken. Please try again\n");
            continue;
        }

        idValid = 1;
    }

    while (nameValid == 0)
    {
        printf("\nEnter in the patient name:\n");
        fgets(name, 50, stdin);
        name[strcspn(name, "\n")] = 0; // Remove newline

        if (strlen(name) == 0)
        {
            printf("\nInvalid input. Please try again\n");
            continue;
        }

        nameValid = 1;
    }

    while (ageValid == 0)
    {
        printf("\nEnter in the age of the patient:\n");
        scanf("%d", &age);
        getchar();

        if (age < 0)
        {
            printf("\nInvalid input. Please try again\n");
            continue;
        }

        ageValid = 1;
    }

    while (diagnosisValid == 0)
    {
        printf("\nEnter in the diagnosis name:\n");
        fgets(diagnosis, 50, stdin);
        diagnosis[strcspn(diagnosis, "\n")] = 0;

        if (strlen(diagnosis) == 0)
        {
            printf("\nInvalid input. Please try again\n");
            continue;
        }

        diagnosisValid = 1;
    }

    while (roomNumberValid == 0)
    {
        printf("\nEnter in the room number:\n");
        scanf("%d", &roomNumber);
        getchar();

        if (roomNumber < 0)
        {
            printf("\nInvalid input. Please try again\n");
            continue;
        }

        roomNumberValid = 1;
    }

    createPatientToList(patientList, id, name, age, diagnosis, roomNumber, NULL);
    writePatientToSaveFile(id, name, age, diagnosis, roomNumber);
}

void createPatientToList(struct LinkedList* patientList,
                         int id,
                         char* name,
                         int age,
                         char* diagnosis,
                         int roomNumber,
                         struct Date* dischargeDate)
{
    struct listNode* createdNode = malloc(sizeof(struct listNode));

    if (createdNode == NULL)
    {
        printf("Could not allocate memory to create a node");
        return;
    }

    createdNode->next = NULL;
    createdNode->patient = malloc(sizeof(struct Patient));
    if (createdNode->patient == NULL)
    {
        printf("Could not allocate memory for a new patient");
        return;
    }

    replaceUnderscoresWithSpaces(name);
    replaceUnderscoresWithSpaces(diagnosis);

    createdNode->patient->id = id;
    createdNode->patient->age = age;
    createdNode->patient->roomNumbers = roomNumber;
    strcpy(createdNode->patient->name, name);
    strcpy(createdNode->patient->diagnosis, diagnosis);
    createdNode->patient->dischargeDate = dischargeDate;

    struct listNode* current = patientList->head;
    struct listNode* prevNode = NULL;

    //If no head, we make the head the created node
    if (patientList->head == NULL)
    {
        patientList->head = createdNode;
        patientList->nodeAmount++;
        return;
    }

    while (current != NULL)
    {
        //Sort Id by ascending when inserting them into the linkedlist
        if (id < current->patient->id)
        {
            prevNode->next = createdNode;
            createdNode->next = current;
            patientList->nodeAmount++;
            return;
        }

        prevNode = current;
        current = current->next;
    }

    //Insert created node to the end of linkedlist when it's the biggest
    //Id within the patient's list
    prevNode->next = createdNode;
    patientList->nodeAmount++;
}


void dischargePatient(struct LinkedList* patientList)
{
    if (patientList->nodeAmount == 0)
    {
        printf("\nPlease register a patient first.\n");
        return;
    }

    int id = 0;
    int dateInitializationStatus = 0;
    struct Date* dischargeDate = malloc(sizeof(struct Date));
    int year = 0;
    int month = 0;
    int day = 0;

    if (dischargeDate == NULL)
    {
        printf("Could not allocated memory for date");
        return;
    }

    printf("\nPlease input the patient's Id to be discharged:\n");
    scanf("%d", &id);

    while (dateInitializationStatus == 0)
    {
        printf("Please input the day the patient was discharged:\n");
        scanf("%d", &day);

        printf("Please input the month the patient was discharged:\n");
        scanf("%d", &month);

        printf("Please input the year the patient was discharged:\n");
        scanf("%d", &year);

        dateInitializationStatus = initializeDate(dischargeDate, day, month, year);

        if (dateInitializationStatus == 0)
        {
            printf("Please try again\n");
        }
    }

    struct listNode* prevNode = NULL;
    struct listNode* currentNode = patientList->head;
    while (currentNode != NULL)
    {
        //Check for matching Id's
        if (currentNode->patient->id == id)
        {
            if (currentNode->patient->dischargeDate != NULL)
            {
                printf("The patient is already discharged\n");
                return;
            }

            currentNode->patient->dischargeDate = dischargeDate;

            printf("\nSuccessfully discharged patient #%d\n", id);
            return;
        }

        prevNode = currentNode;
        currentNode = currentNode->next;
    }

    printf("No patient with that Id was found\n");
}

void ReportsAndAnalyticsMenu(const struct LinkedList* patientList)
{
    int input = 0;

    while (input != 6)
    {
        printf("\nREPORTING AND ANALYTICS\n"
            "1. View All Patients\n"
            "2. Patients Discharged by Day, Week, And Month\n"
            "3. Patients Discharged on a Specific Day\n"
            "4. Discharge Patient Statistics\n"
            "5. Room Usage Statistics\n"
            "6. Go back\n"
            "Please input your option:\n");

        scanf("%d", &input);

        switch (input)
        {
        case 1:
            viewAllPatients(patientList);
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        }
    }
}

void viewAllPatients(const struct LinkedList* patientList)
{
    if (patientList->nodeAmount == 0)
    {
        printf("\nPlease register a patient first\n");
        return;
    }

    printf("\n");

    struct listNode* currentNode = patientList->head;
    while (currentNode != NULL)
    {
        struct Patient* nodePatient = currentNode->patient;
        char dischargedDateString[50] = "Not Discharged";

        dateToString(nodePatient->dischargeDate, dischargedDateString);

        printf("Id: %d | Name: %s | Age: %d | Diagnosis: %s | Room Number: %d | Date discharged: %s\n",
               nodePatient->id, nodePatient->name, nodePatient->age, nodePatient->diagnosis, nodePatient->roomNumbers,
               dischargedDateString);

        strcpy(dischargedDateString, "");
        currentNode = currentNode->next;
    }

    printf("\n");
}

void searchPatientMenu(struct LinkedList* patientList)
{
    int input = 0;

    while (input != 3)
    {
        printf("SEARCH PATIENT MENU\n"
            "1. Search by Id\n"
            "2. Search by Name\n"
            "3. Go back\n"
            "Please input your option:\n");

        scanf("%d", &input);

        switch (input)
        {
        case 1:
            searchPatientByID(patientList);
            break;
        case 2:
            searchPatientByName(patientList);
            break;
        }
    }
}

void searchPatientByID(struct LinkedList* patientList)
{
    int id = -1;
    const struct listNode* currentNode;

    while (id < 0)
    {
        printf("\nPlease input the Patient's Id:\n");
        scanf("%d", &id);

        if (id < 0)
        {
            printf("Please input a valid Id");
        }
    }

    currentNode = patientList->head;

    while (currentNode != NULL)
    {
        if (id == currentNode->patient->id)
        {
            printf("Id: %d | Name: %s | Age: %d | Diagnosis: %s | Room Number: %d\n",
                   currentNode->patient->id, currentNode->patient->name, currentNode->patient->age,
                   currentNode->patient->diagnosis, currentNode->patient->roomNumbers);
            return;
        }
    }

    printf("Patient was not found");
}

void searchPatientByName(struct LinkedList* patientList)
{
    char name[50];
    int patientFound = 0;
    struct listNode* currentNode;

    printf("\nEnter Patient Name:\n");

    getchar();
    fgets(name, 50, stdin);
    name[strcspn(name, "\n")] = 0;

    currentNode = patientList->head;
    while (currentNode != NULL)
    {
        if (strcmp(name, currentNode->patient->name) == 0)
        {
            printf("Id: %d | Name: %s | Age: %d | Diagnosis: %s | Room Number: %d\n",
                   currentNode->patient->id, currentNode->patient->name, currentNode->patient->age,
                   currentNode->patient->diagnosis, currentNode->patient->roomNumbers);
            patientFound = 1;
        }

        currentNode = currentNode->next;
    }

    if (!patientFound)
    {
        printf("No patients were found\n");
    }
}

//Bunch of date functions below
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

int initializeDate(struct Date* date,
                   int day,
                   int month,
                   int year)
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
    char *endptr;

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
        }

        if (*endptr != '\0') {
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

int validateDate(int day,
                 int month,
                 int year)
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
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (day < 1 || day > daysInMonth[month - 1])
    {
        printf("Invalid Day\n");
        return 0;
    }

    return 1;
}
