//
// Created by allan on 2025-03-28.
//

#ifndef HOSPITAL_H
#define HOSPITAL_H

#define MAX_LENGTH 200

struct Patient
{
    int id;
    char name[MAX_LENGTH];
    int age;
    char diagnosis[MAX_LENGTH];
    int roomNumbers;
    struct Date* dischargeDate;
};

struct Date
{
    int day;
    int month;
    int year;
};

struct listNode
{
    struct Patient* patient;
    struct listNode* next;
};

struct LinkedList
{
    struct listNode* head;
    int nodeAmount;
};

void addPatient(struct LinkedList* patientList);

void createPatientToList(struct LinkedList* patientList,
                         int id,
                         char* name,
                         int age,
                         char* diagnosis,
                         int roomNumber,
                         struct Date* dischargeDate);

void writePatientToSaveFile(int id,
                            char* name,
                            int age,
                            char* diagnosis,
                            int roomNumber);

int validateDate(int day,
                 int month,
                 int year);

int initializeDate(struct Date* date,
                   int day,
                   int month,
                   int year);

void dateToString(const struct Date* date,
                  char* string);

void parseDate(struct Date** date,
               char* dateString);

void loadSaveFile(struct LinkedList* patientList);

void writeAllPatientsToSaveFile(const struct LinkedList* patientList);

void ReportsAndAnalyticsMenu(const struct LinkedList* patientList);

void viewAllPatients(const struct LinkedList* patientList);

void searchPatientMenu(struct LinkedList* patientList);

void searchPatientByID(struct LinkedList* patientList);

void searchPatientByName(struct LinkedList* patientList);

void dischargePatient(struct LinkedList* patientList);

void manageDoctors();

void addDoctor();

void viewAllDoctors();

void manageDoctorsSchedule();

void viewDoctorsSchedule();


#endif //HOSPITAL_H
