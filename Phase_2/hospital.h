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
