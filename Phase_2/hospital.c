//
// Created by allan on 2025-03-28.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "hospital.h"

int main()
{
    struct LinkedList patientList = {NULL, 0};
    int input = 1;

    //Load from save file here

    while (input != 6)
    {
        printf("\nMAIN MENU\n"
            "1. Add Patient Record\n"
            "2. Reporting and Analytics Menu\n"
            "3. Search Patient by ID or Name\n"
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
            break;
        case 4:
            dischargePatient(&patientList);
            break;
        case 5:
            break;
        }
    }

    //TODO: free up the nodes within the linkedlist
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

    createdNode->patient->id = id;
    createdNode->patient->age = age;
    createdNode->patient->roomNumbers = roomNumber;
    strcpy(createdNode->patient->name, name);
    strcpy(createdNode->patient->diagnosis, diagnosis);

    if (patientList->head == NULL)
    {
        patientList->head = createdNode;
        patientList->nodeAmount++;
        return;
    }

    struct listNode* current = patientList->head;

    while (current->next != NULL)
    {
        // printf("%s\n", current->patient->name);
        current = current->next;
    }

    current->next = createdNode;
}

void dischargePatient(struct LinkedList* patientList)
{
    if (patientList->nodeAmount == 0)
    {
        printf("\nPlease register a patient first.\n");
        return;
    }

    int id = 0;

    printf("\nPlease input the patient's Id to be discharged:\n");
    scanf("%d", &id);

    struct listNode* prevNode = NULL;
    struct listNode* currentNode = patientList->head;
    while (currentNode != NULL)
    {
        if (currentNode->patient->id == id)
        {
            if (prevNode == NULL)
            {
                patientList->head = currentNode->next;
            } else
            {
                prevNode->next = currentNode->next;
            }

            patientList->nodeAmount--;
            printf("Successfully delete patient #%d\n", id);
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
        printf("Id: %d | Name: %s | Age: %d | Diagnosis: %s | Room Number: %d\n",
               nodePatient->id, nodePatient->name, nodePatient->age, nodePatient->diagnosis, nodePatient->roomNumbers);

        currentNode = currentNode->next;
    }

    printf("\n");
}

void searchPatient(struct LinkedList* patientList);

void searchPatientByID(struct LinkedList* patientList);

void searchPatientByName(struct LinkedList* patientList);

void dischargePatient(struct LinkedList* patientList);
