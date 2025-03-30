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
    int maxPatients = 10;
    struct Patient** patients = malloc(maxPatients * sizeof(struct Patient*));
    int patientAmount = 0;
    int input = 1;

    if (patients == NULL)
    {
        printf("Could not allocate memory for patients");
        free(patients);
        return 1;
    }

    while (input != 6)
    {
        printf("\n1. Add Patient Record\n"
            "2. View All Patients\n"
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
            addPatient(&patients, &patientAmount, &maxPatients);
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        }
    }
}

int validateId(struct Patient** patients,
               int patientAmount,
               int id)
{
    for (int i = 0; i < patientAmount; i++)
    {
        printf("%s\n", patients[i]->name);
        if (patients[i]->id == id)
        {
            return 1;
        }
    }

    return 0;
}

void addPatient(struct Patient*** patients,
                int* patientAmount,
                int* maxPatientAmount)
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

        if (id < 0 || validateId(*patients, *patientAmount, id))
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

    (*patients)[*patientAmount] = malloc(sizeof(struct Patient));

    if (patients[*patientAmount] == NULL) {
        printf("Memory allocation failed\n");
        return;
    }

    if (*maxPatientAmount <= *patientAmount)
    {
        printf("allocated");

        struct Patient** temp = realloc(*patients, sizeof(struct Patient*) * (*patientAmount + 10));
        if (temp == NULL)
        {
            printf("Could not allocate more memory");
            return;
        }
        maxPatientAmount += 10;
        *patients = temp;
    }

    (*patients)[*patientAmount]->id = id;
    (*patients)[*patientAmount]->age = age;
    strcpy((*patients)[*patientAmount]->diagnosis, diagnosis);
    strcpy((*patients)[*patientAmount]->name, name);
    (*patients)[*patientAmount]->roomNumbers = roomNumber;

    (*patientAmount)++;
}
